/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:52:56 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/09 14:41:39 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcChannel.hpp"

int		IrcChannel::is_ops(std::string nick){
	for (std::vector<IrcUser>::iterator it = ops.begin(); it < ops.end(); it++)
	{
		if (it->getNick() == nick)
			return (1);
	}
	return (0);
}

int	IrcChannel::is_user(std::string user){
	for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++){
		if (it->getNick() == user)
			return (1);
	}
	return (0);
}

IrcChannel::IrcChannel(std::string title, IrcUser user) : title(title), creator(user)
{
    ops.push_back(creator);
    users.push_back(creator);
    max_users = 0;
    topicperm = 0;
    invite = 0;
    symbol = '=';
}

IrcChannel::~IrcChannel() {}

void            IrcChannel::setTitle(std::string title)
{
    this->title = title;
}

std::string     IrcChannel::getTitle() const
{
    return (title);
}

void            IrcChannel::addUser(IrcUser user)
{
    users.push_back(user);
}

void            IrcChannel::deleteUser(int userfd)
{
    for (int i = 0; i < (int)users.size(); i++)
    {
        if (userfd == users[i].getFd())
            users.erase(users.begin() + i);
    }
    for (int i = 0; i < (int)ops.size(); i++)
    {
        if (userfd == ops[i].getFd())
            ops.erase(ops.begin() + i);
    }
}

void            IrcChannel::addOp(IrcUser user)
{
    ops.push_back(user);
}

void            IrcChannel::deleteOP(int userfd)
{
    for (int i = 0; i < (int)ops.size(); i++)
    {
        if (userfd == ops[i].getFd())
            ops.erase(ops.begin() + i);
    }
}

void            IrcChannel::setMaxUsers(int max)
{
    this->max_users = max;
}

int             IrcChannel::getMaxUsers() const
{
    return (max_users);
}

void            IrcChannel::setInvite(bool invite)
{
    this->invite = invite;
}

bool            IrcChannel::getInvite() const
{
    return (invite);
}

void            IrcChannel::setTopicPerm(bool topic)
{
    this->topicperm = topic;
}

bool            IrcChannel::getTopicPerm() const
{
    return (topicperm);
}

std::vector<IrcUser>	*IrcChannel::getUsers()
{
	return (&users);
}

std::vector<IrcUser>	*IrcChannel::getOps()
{
	return (&ops);
}

std::string             IrcChannel::getList()
{
    std::string     dest;

    for (std::vector<IrcUser>::iterator it = users.begin(); it < users.end(); it++)
    {
        if (creator.getNick() == (*it).getNick())
            dest += '~';
        else if (is_ops((*it).getNick()))
            dest += '@';
        dest += (*it).getNick();
        if (it + 1 < users.end())
            dest += ' ';
    }
    return dest;
}

char                    IrcChannel::getSymbol()
{
    return symbol;
}

void                    IrcChannel::setSymbol(char symb)
{
    symbol = symb;
}

std::string             IrcChannel::getTopic()
{
    return (topic);
}

void                    IrcChannel::setTopic(std::string str)
{
    topic = str;
}

std::string             IrcChannel::getPwd()
{
    return (pwd);
}

void                    IrcChannel::setPwd(std::string p)
{
    pwd = p;
}