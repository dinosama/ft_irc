/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:52:56 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/06 23:38:24 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcChannel.hpp"

IrcChannel::IrcChannel(std::string title, IrcUser user) : title(title), creator(user)
{
    ops.push_back(creator);
    users.push_back(creator);
    max_users = 0;
    topic = 1;
    invite = 0;
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
    ops.push_back(user);
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
    this->topic = topic;
}

bool            IrcChannel::getTopicPerm() const
{
    return (topic);
}

std::vector<IrcUser>	*IrcChannel::getUsers()
{
	return (&users);
}

std::vector<IrcUser>	*IrcChannel::getOps()
{
	return (&ops);
}