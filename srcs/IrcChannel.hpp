/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcChannel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:39:52 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/09 15:11:57 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCCHANNEL_HPP
#define IRCCHANNEL_HPP

#include <iostream>
#include <vector>
#include "IrcUser.hpp"
#include <ctime>

class IrcChannel
{
    private:
        std::string             title;
        std::string             topic;
		std::string				pwd;
        std::vector<IrcUser>    users;
        std::vector<IrcUser>    ops;
        IrcUser                 creator;
        char                    symbol;
        int                     max_users;
        bool                    invite;
        bool                    topicperm;
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
        char            getSymbol();
        void            setSymbol(char symb);
        std::string     getList();
        std::string     getPwd();
		std::vector<IrcUser>	*getUsers();
		std::vector<IrcUser>	*getOps();
        std::string     getTopic();
        void            setTopic(std::string str);
        void            setPwd(std::string p);
};

#endif