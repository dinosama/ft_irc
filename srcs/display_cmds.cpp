/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_cmds.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:34:20 by motaouss          #+#    #+#             */
/*   Updated: 2023/12/09 14:40:16 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUser.hpp"
#include "IrcServ.hpp"
#include "IrcChannel.hpp"
#include "Token.hpp"
#include "numerical_replies.hpp"
#include <ctime>

void	send_one(std::string msg, IrcUser &user)
{
	user.getList()->push_back(msg);
}

void	IrcServ::send_channel(std::string msg, IrcChannel &channel, IrcUser *nothim)
{
	for (std::vector<IrcUser>::iterator it = channel.getUsers()->begin(); it < channel.getUsers()->end(); it++)
	{
		if (nothim == NULL || nothim->getNick() != (*it).getNick())
		{
			IrcUser *user = is_user((*it).getNick());
			user->getList()->push_back(msg);
		}
	}
}

void	send_all(std::string msg, std::vector<IrcUser> &users)
{
	for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++)
		(*it).getList()->push_back(msg);
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
	return (0);
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
		send_one(ERR_NICKNAMEINUSE((*t->getParam())[0], (*t->getParam())[0]), user);
	else
	{
		if (user.getNick() == "")
			user.setRegplus();
		user.setNick((*t->getParam())[0]);
		send_one(RPL_NICK(oldnick, user.getName(), (*t->getParam())[0]), user);
		if (user.getReg() == 2)
		{
			send_one(RPL_WELCOME(user_id(user.getNick(), user.getName()), user.getNick()), user);
			send_one(RPL_YOURHOST(user.getNick(), "ft_ircnul", "2.8"), user);
			send_one(RPL_MYINFO(user.getNick(), "localhost", "2.8", "io", "koitl", "ko"), user);
			send_one(RPL_ISUPPORT(user.getNick(), "CHANNELLEN=32 NICKLEN=9 TOPICLEN=307"), user);
		}
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

IrcUser		*IrcServ::is_user(std::string user)
{
	for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++)
	{
		if ((*it).getNick() == user)
			return (&(*(it)));
	}
	return (NULL);
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
	if (user.getReg() == 0)
		send_one(ERR_NOTREGISTERED(user.getNick()), user);
	else if (t->getNparam() < 2)
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
		send_channel(RPL_KICK(user_id(user.getNick(), user.getName()), (*t->getParam())[0], (*t->getParam())[1], reason), *is_channel((*t->getParam())[0]), NULL);
		delete_user(find_userfd((*t->getParam())[1]));
	}
}

void	IrcServ::user_cmd(Token *t, IrcUser &user){
	if (t->getNparam() < 4)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else if (!(user.getName().empty()))
		send_one(ERR_ALREADYREGISTERED(user.getNick()), user);
	else if (pwd != user.getPwd())
		send_one(ERR_PASSWDMISMATCH(user.getNick()), user);
	else
	{
		user.setName((*t->getParam())[0]);
		user.setReal((*t->getParam())[3]);
		user.setRegplus();
		if (user.getReg() == 2)
		{
			send_one(RPL_WELCOME(user_id(user.getNick(), user.getName()), user.getNick()), user);
			send_one(RPL_YOURHOST(user.getNick(), "ft_ircnul", "2.8"), user);
			send_one(RPL_MYINFO(user.getNick(), "localhost", "2.8", "io", "koitl", "ko"), user);
			send_one(RPL_ISUPPORT(user.getNick(), "CHANNELLEN=32 NICKLEN=9 TOPICLEN=307"), user);
		}
	}
}

void	IrcServ::prvmsg(Token *t, IrcUser &user)
{
	bool	channel = 0;

	if (t->getNparam() > 0 && (*t->getParam())[0][0] == '#')
		channel = 1;
	if (user.getReg() == 0)
		send_one(ERR_NOTREGISTERED(user.getNick()), user);
	else if (t->getNparam() < 1)
		send_one(ERR_NORECIPIENT(user.getNick()), user);
	else if (t->getNparam() < 2)
		send_one(ERR_NOTEXTTOSEND(user.getNick()), user);
	else if (channel && is_channel((*t->getParam())[0]) == NULL)
		send_one(ERR_CANNOTSENDTOCHAN(user.getNick(), (*t->getParam())[0]), user);
	else if (!channel && is_user((*t->getParam())[0]) == NULL)
		send_one(ERR_NOSUCHNICK(user.getNick(), (*t->getParam())[0]), user);
	else if (channel)
	{
		std::cout << "CHANNELLLLL: " << channel << std::endl;
		send_channel(RPL_PRIVMSG(user.getNick(), user.getName(), (*t->getParam())[0], (*t->getParam())[1]), *is_channel((*t->getParam())[0]), &user);
	}
	else if (!channel)
		send_one(RPL_PRIVMSG(user.getNick(), user.getName(), (*t->getParam())[0], (*t->getParam())[1]), *is_user((*t->getParam())[0]));
}

void	IrcServ::invite(Token *t, IrcUser &user){
	if (user.getReg() == 0)
		send_one(ERR_NOTREGISTERED(user.getNick()), user);
	if (t->getNparam() < 2)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else if (is_channel((*t->getParam())[1]) == NULL)
		send_one(ERR_NOSUCHCHANNEL(user.getNick(), (*t->getParam())[1]), user);
	else if (is_channel((*t->getParam())[1])->is_user(user.getNick()))
		send_one(ERR_NOTONCHANNEL(user.getNick(), (*t->getParam())[1]), user);
	else if (is_channel((*t->getParam())[1])->is_ops(user.getNick()))
		send_one(ERR_CHANOPRIVSNEEDED(user.getNick(), (*t->getParam())[1]), user);
	else if (is_channel((*t->getParam())[1])->is_user((*t->getParam())[0]))
		send_one(ERR_USERONCHANNEL(user.getNick(), (*t->getParam())[0], (*t->getParam())[1]), user);
	else
	{
		is_channel((*t->getParam())[1])->addUser(*is_user((*t->getParam())[0]));
		send_one(RPL_INVITING(user_id(user.getNick(), user.getName()), user.getNick(), (*t->getParam())[0], (*t->getParam())[1]), user);
		send_one(RPL_INVITE(user_id(user.getNick(), user.getName()), (*t->getParam())[0], (*t->getParam())[1]), *is_user((*t->getParam())[0]));
	}
}

void	IrcServ::pong(Token *t, IrcUser &user)
{
	if (t->getNparam() < 1)
		send_one(RPL_PONG(user_id(user.getNick(), user.getName()), ""), user);
	if (t->getNparam() == 1)
		send_one(RPL_PONG(user_id(user.getNick(), user.getName()), (*t->getParam())[0]), user);
}

void	IrcServ::join(Token *t, IrcUser &user)
{
	if (user.getReg() == 0)
		send_one(ERR_NOTREGISTERED(user.getNick()), user);
	else if (t->getNparam() < 1)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else if (is_channel((*t->getParam())[0]) == NULL)
	{
		add_channel((*t->getParam())[0], user);
		send_one(RPL_JOIN(user_id(user.getNick(), user.getName()), (*t->getParam())[0]), user);
		send_one(RPL_NAMREPLY(user.getNick(), is_channel((*t->getParam())[0])->getSymbol(), (*t->getParam())[0], is_channel((*t->getParam())[0])->getList()), user);
		send_one(RPL_ENDOFNAMES(user.getNick(), (*t->getParam())[0]), user);
	}
	else if (is_channel((*t->getParam())[0])->getPwd() != "")
	{
		if (t->getNparam() < 2 || (*t->getParam())[1] != is_channel((*t->getParam())[0])->getPwd())
			send_one(ERR_BADCHANNELKEY(user.getNick(), (*t->getParam())[0]), user);
	}
	else
	{
		send_channel(RPL_JOIN(user.getNick(), (*t->getParam())[0]), *is_channel((*t->getParam())[0]), &user);
		if (is_channel((*t->getParam())[0])->getTopic() != "")
			send_one(RPL_TOPIC(user.getNick(), (*t->getParam())[0], is_channel((*t->getParam())[0])->getTopic()), user);
		is_channel((*t->getParam())[0])->addUser(user);
		send_one(RPL_NAMREPLY(user.getNick(), is_channel((*t->getParam())[0])->getSymbol(), (*t->getParam())[0], is_channel((*t->getParam())[0])->getList()), user);
		send_one(RPL_ENDOFNAMES(user.getNick(), (*t->getParam())[0]), user);
	}
}

void	IrcServ::topic(Token *t, IrcUser &user)
{
	if (user.getReg() == 0)
		send_one(ERR_NOTREGISTERED(user.getNick()), user);
	else if (!is_channel((*t->getParam())[0]))
		send_one(ERR_NOSUCHCHANNEL(user.getNick(), (*t->getParam())[0]), user);
	else if (!is_channel((*t->getParam())[0]) && t->getNparam() > 0 && !is_channel((*t->getParam())[0])->is_user(user.getNick()))
		send_one(ERR_NOTONCHANNEL(user.getNick(), (*t->getParam())[0]), user);
	else if (t->getNparam() < 2)
	{
		if (is_channel((*t->getParam())[0])->getTopic() == "")
			send_one(RPL_NOTOPIC(user.getNick(), (*t->getParam())[0]), user);
		else
			send_one(RPL_TOPIC(user.getNick(), (*t->getParam())[0], is_channel((*t->getParam())[0])->getTopic()), user);
	}
	else
	{
		if (is_channel((*t->getParam())[0])->getTopicPerm() == 1 && !is_channel((*t->getParam())[0])->is_ops(user.getNick()))
			send_one(ERR_CHANOPRIVSNEEDED(user.getNick(), (*t->getParam())[0]), user);
		else
		{
			is_channel((*t->getParam())[0])->setTopic((*t->getParam())[1]);
			send_channel(RPL_TOPIC(user.getNick(), (*t->getParam())[0], is_channel((*t->getParam())[0])->getTopic()), *is_channel((*t->getParam())[0]), NULL);
		}
	}
}

void	IrcServ::mode(Token *t, IrcUser &user)
{
	std::string mode;
	IrcChannel	*chan;
	std::string	parameters;
	char		sign;
	int			i = 0;

	if (t->getNparam() < 1)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else if ((*t->getParam())[0][0] != '#')
	{
		if (is_user((*t->getParam())[0]) == NULL)
			send_one(ERR_NOSUCHNICK(user.getNick(), (*t->getParam())[0]), user);
		else if ((*t->getParam())[0] != user.getNick())
			send_one(ERR_USERSDONTMATCH(user.getNick()), user);
		else if (t->getNparam() < 2)
		{
			if (user.getop())
				mode = "+o";
			if (user.getinv())
				mode = "+i";
			if (user.getinv() && user.getop())
				mode = "+oi";
			send_one(RPL_UMODEIS(user.getNick(), mode), user);
		}
		else
		{
			if ((*t->getParam())[1] == "+i")
			{
				user.setinv(1);
				send_one(MODE_USERMSG(user.getNick(), "+i"), user);
			}
			else if ((*t->getParam())[1] == "-i")
			{
				user.setinv(0);
				send_one(MODE_USERMSG(user.getNick(), "-i"), user);
			}
			else if ((*t->getParam())[1] == "-o")
			{
				user.setop(0);
				send_one(MODE_USERMSG(user.getNick(), "-o"), user);
			}
			else
				send_one(ERR_UMODEUNKNOWNFLAG(user.getNick()), user);
		}
	}
	else
	{
		if (is_channel((*t->getParam())[0]) == NULL)
			send_one(ERR_NOSUCHCHANNEL(user.getNick(), (*t->getParam())[0]), user);
		chan = is_channel((*t->getParam())[0]);
		if (t->getNparam() < 2)
			return ;
		for (std::string::iterator it = (*t->getParam())[1].begin(); it < (*t->getParam())[1].end(); it++)
		{
			if (*it == '+' || *it == '-')
			{
				sign = *it;
				mode += sign;
			}
			else if (sign == '+')
			{
				if (*it == 'i')
				{
					mode += 'i';
					chan->setSymbol('*');
					chan->setInvite(1);
				}
				if (*it == 't')
				{
					mode += 't';
					chan->setTopicPerm(1);
				}
				if (*it == 'k')
				{
					if (t->getNparam() < i + 3)
						continue;
					mode += 'k';
					chan->setPwd((*t->getParam())[2 + i]);
					parameters += ' ' + (*t->getParam())[2 + i];
					i++;
				}
				if (*it == 'o')
				{
					if (t->getNparam() < i + 3)
						continue;
					mode += 'o';
					chan->addOp(*is_user((*t->getParam())[2 + i]));
					parameters += ' ' + (*t->getParam())[2 + i];
					i++;
				}
				if (*it == 'l')
				{
					if (t->getNparam() < i + 3)
						continue;
					mode += 'l';
					chan->setMaxUsers(atoi((*t->getParam())[2 + i].c_str()));
					parameters += ' ' + (*t->getParam())[2 + i];
					i++;
				}
			}
			else if (sign == '-')
			{
				if (*it == 'i')
				{
					mode += 'i';
					chan->setSymbol('=');
					chan->setInvite(0);
				}
				if (*it == 't')
				{
					mode += 't';
					chan->setTopicPerm(0);
				}
				if (*it == 'k')
				{
					if (t->getNparam() < i + 3)
						continue;
					mode += 'k';
					chan->setPwd("");
				}
				if (*it == 'o')
				{
					if (t->getNparam() < i + 3)
						continue;
					mode += 'o';
					chan->deleteOP(*is_user((*t->getParam())[2 + i])->getBuf());
					parameters += ' ' + (*t->getParam())[2 + i];
					i++;
				}
				if (*it == 'l')
				{
					if (t->getNparam() < i + 3)
						continue;
					mode += 'l';
					chan->setMaxUsers(0);
				}
			}
		}
		mode = mode + parameters;
		send_channel(MODE_CHANNELMSG((*t->getParam())[0], mode), *chan, NULL);
	}
}

/*
— i : Définir/supprimer le canal sur invitation uniquement
— t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
— k : Définir/supprimer la clé du canal (mot de passe)
— o : Donner/retirer le privilège de l’opérateur de canal
— l : Définir/supprimer la limite d’utilisateurs pour le canal
*/

void	IrcServ::pass(Token *t, IrcUser &user)
{
	if (user.getReg() == 1)
		send_one(ERR_ALREADYREGISTERED(user.getNick()), user);
	if (t->getNparam() != 1)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	else
		user.setPwd((*t->getParam())[0]);
}

void	IrcServ::oper(Token *t, IrcUser &user)
{
	if (t->getNparam() < 2)
		send_one(ERR_NEEDMOREPARAMS(user.getNick(), t->getCommand()), user);
	if ((*t->getParam())[0] == user.getNick() && (*t->getParam())[1] == pwd)
	{
		user.setop(1);
		send_one(RPL_YOUREOPER(user.getNick()), user);
	}
	else
		send_one(ERR_PASSWDMISMATCH(user.getNick()), user);
}