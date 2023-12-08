/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_cmds.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:34:20 by motaouss          #+#    #+#             */
/*   Updated: 2023/12/07 04:22:23 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUser.hpp"
#include "IrcServ.hpp"
#include "IrcChannel.hpp"
#include "Token.hpp"
#include "numerical_replies.hpp"

void	send_one(std::string msg, IrcUser &user)
{
	if (send(user.getFd(), msg.data(), msg.size(), 0) == -1)
	{
		std::cerr << "error: send (send_one)" << std::endl;
		return ;
	}
}

void	send_channel(std::string msg, IrcChannel &channel)
{
	for (std::vector<IrcUser>::iterator it = channel.getUsers()->begin(); it < channel.getUsers()->end(); it++)
	{
		if (send((*it).getFd(), msg.data(), msg.size(), 0) == -1)
		{
			std::cerr << "error: send (send_channel) fd: " << (*it).getFd() << std::endl;
			continue ;
		}
	}
}

void	send_all(std::string msg, std::vector<IrcUser> &users)
{
	for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++)
	{
		if (send((*it).getFd(), msg.data(), msg.size(), 0) == -1)
		{
			std::cerr << "error: send (send_channel) fd: " << (*it).getFd() << std::endl;
			continue ;
		}
	}
}

int	check_nick2(char c){
	if (std::isalnum(c) || c == '[' ||
	 c == ']' || c == '{' ||
	  c == '}' || c == '\\' || c == '|')
	  return (1);
	else
		return (0);
}

int	check_nick(std::string str){
	if ((str[0] >= '0' && str[0] <= '9') || str[0] == '#' || (str[0] == '&' && str[1] == '#'))
		return (1);
	for (int i = 0; check_nick2(str[i]); i++){
		if (!str[i + 1])
			return (0);
	}
	return (1);
}

int		IrcServ::used_nickname(std::string str){
	int i = 1;
	for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++){
		std::cout << i << " nick: " << it->getNick() << std::endl;
		i++;
		if (it->getNick() == str)
			return (1);
	}
	return (0);
}

void		IrcServ::nickname(Token *t, IrcUser &user, IrcServ &serv){
	std::string oldnick = user.getNick();

	if (t->getNparam() == 0)
		send_one(ERR_NONICKNAMEGIVEN(user.getNick()), user);
	else if (check_nick((*t->getParam())[0]))
		send_one(ERR_ERRONEUSNICKNAME(user.getNick(), (*t->getParam())[0]), user);
	else if (serv.used_nickname((*t->getParam())[0]))
	{
		std::cout << (*t->getParam())[0] << std::endl;
		send_one(ERR_NICKNAMEINUSE((*t->getParam())[0], (*t->getParam())[0]), user);
	}
	else
	{
		std::cout << (*t->getParam())[0] << std::endl;
		user.setNick((*t->getParam())[0]);
		send_one(RPL_NICK(oldnick, user.getName(), (*t->getParam())[0]), user);
	}
}

IrcChannel	*IrcServ::is_channel(std::string channel){
	IrcChannel *tmp = NULL;
	for (std::vector<IrcChannel>::iterator it = channels.begin(); it != channels.end(); ++it){
		if (it->getTitle() == channel)
			tmp = &(*it);
	}
	return (tmp);
}

int		IrcChannel::is_ops(std::string nick){
	for (std::vector<IrcUser>::iterator it = ops.begin(); it != ops.end(); ++it){
		if (it->getNick() == nick)
			return (0);
	}
	return (1);
}

int	IrcChannel::is_user(std::string user){
	for (std::vector<IrcUser>::iterator it = users.begin(); it != users.end(); ++it){
		if (it->getNick() == user)
			return (0);
	}
	return (1);
}

int		IrcServ::find_userfd(std::string nick)
{
	for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++)
	{
		if (nick == (*it).getNick())
			return ((*it).getFd());
	}
	return (-1);
}

void 	IrcServ::kick(Token *t, IrcUser &user){
	std::string reason;
	
	if (t->getNparam() == 3)
		reason = (*t->getParam())[2];
	else
		reason = "someone don't like you...";
	if (t->getNparam() < 2)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else if (is_channel((*t->getParam())[0]) == NULL)
		send_one(ERR_NOSUCHCHANNEL(user.getNick(), (*t->getParam())[0]), user);
	else if (is_channel((*t->getParam())[0])->is_ops(user.getNick()))
		send_one(ERR_CHANOPRIVSNEEDED(user.getNick(), (*t->getParam())[0]), user);
	else if (is_channel((*t->getParam())[0])->is_user((*t->getParam())[1]))
		send_one(ERR_USERNOTINCHANNEL(user.getNick(), (*t->getParam())[1], (*t->getParam())[0]), user);
	else if (is_channel((*t->getParam())[0])->is_user(user.getNick()))
		send_one(ERR_NOTONCHANNEL(user.getNick(), (*t->getParam())[0]), user);
	else
	{
		send_channel(RPL_KICK(user.getNick(), (*t->getParam())[0], (*t->getParam())[1], reason), *is_channel((*t->getParam())[0]));
		delete_user(find_userfd((*t->getParam())[1]));
	}
}

void	user_cmd(Token *t, IrcUser &user){
	if (t->getNparam() < 4)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else if (!(user.getName().empty()))
		send_one(ERR_ALREADYREGISTERED(user.getNick()), user);
	else
	{
		user.setName((*t->getParam())[0]);
		user.setReal((*t->getParam())[3]);
		user.setReg(1);
	}
}
