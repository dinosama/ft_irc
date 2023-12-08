/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motaouss <motaouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:39:52 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/06 23:37:30 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCCHANNEL_HPP
#define IRCCHANNEL_HPP

#include <iostream>
#include <vector>
#include "IrcUser.hpp"

class IrcChannel
{
    private:
        std::string             title;
		std::string				pwd;
        std::vector<IrcUser>    users;
        std::vector<IrcUser>    ops;
        IrcUser                 creator;
        int                     max_users;
        bool                    invite;
        bool                    topic;
    public:
        IrcChannel(std::string title, IrcUser user);
        ~IrcChannel();
        void            addUser(IrcUser user);
        void            deleteUser(int userfd);
        void            addOp(IrcUser user);
        void            deleteOP(int userfd);
        void            setTitle(std::string title);
        std::string     getTitle() const;
        void            setMaxUsers(int max);
        int             getMaxUsers() const;
        void            setInvite(bool invite);
        bool            getInvite() const;
        void            setTopicPerm(bool topic);
        bool            getTopicPerm() const;
        int             is_ops(std::string  nick);
        int             is_user(std::string user);
		std::vector<IrcUser>	*getUsers();
		std::vector<IrcUser>	*getOps();
};

#endif