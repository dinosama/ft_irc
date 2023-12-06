/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_cmds.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motaouss <motaouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:34:20 by motaouss          #+#    #+#             */
/*   Updated: 2023/12/06 21:41:06 by motaouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUser.hpp"
#include "IrcServ.hpp"
#include "Token.hpp"
#include "numerical_replies.hpp"

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
	for (std::vector<IrcUser>::iterator it = users.begin(); it != users.end(); ++it){
		if (it->getName() == str)
			return (1);
	}
	return (0);
}

std::string		nickname(Token *t, IrcUser &user, IrcServ &serv){
	if (t->nparam == 0)
		return (ERR_NONICKNAMEGIVEN(user.getNick()));
	else if (check_nick(t->parameters[0]))
		return (ERR_ERRONEUSNICKNAME(user.getNick(), t->parameters[0]));
	else if (serv.used_nickname(t->parameters[0]))
		return (ERR_NICKNAMEINUSE(user.getNick(), t->parameters[0]));
	else
		return (RPL_NICK(user.getNick(), user.getName(), t->parameters[0]));
}

IrcChannel	*IrcServ::is_channel(std::string channel){
	IrcChannel *tmp = NULL;
	for (std::vector<IrcChannel>::iterator it = channels.begin(); it != channels.end(); ++it){
		if (it->getTitle() == channel)
			tmp = &(*it);
	}
	return (tmp);
}

int	IrcChannel::is_ops(std::string nick){
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

std::string IrcServ::kick(Token *t, IrcUser &user){
	if (t->nparam < 2)
		return (ERR_NEEDMOREPARAMS(user.getNick(), t->command));
	else if (is_channel(t->parameters[0]) == NULL)
		return (ERR_NOSUCHCHANNEL(user.getNick(), t->parameters[0]));
	else if (is_channel(t->parameters[0])->is_ops(user.getNick()))
		return (ERR_CHANOPRIVSNEEDED(user.getNick(), t->parameters[0]));
	else if (is_channel(t->parameters[0])->is_user(t->parameters[1]))
		return (ERR_USERNOTINCHANNEL(user.getNick(), t->parameters[1], t->parameters[0]));
	else if (is_channel(t->parameters[0])->is_user(user.getNick()))
		return (ERR_NOTONCHANNEL(user.getNick(), t->parameters[0]));
}

std::string user_cmd(Token *t, IrcUser &user){
	if (t->nparam < 2)
		return (ERR_NEEDMOREPARAMS(user.getNick(), t->command));
	else if (!(user.getName().empty()))
		return (ERR_ALREADYREGISTERED(user.getNick()));
}



