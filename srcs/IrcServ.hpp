/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motaouss <motaouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 04:48:27 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/07 04:59:49 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "IrcUser.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <stdlib.h>
#include <cerrno>
#include <fcntl.h>
#include "Token.hpp"
#include "IrcChannel.hpp"
#include "algorithm"
extern bool power;

class IrcServ
{
    private:
        int                             sockfd;
        int                             error;
        std::string                     pwd;
        std::vector<IrcUser>            users;
        std::vector<pollfd>             pfds;
        std::vector<IrcChannel>         channels;
    public:
        IrcServ(std::string port, std::string pwd);
        ~IrcServ();
        void    	add_user(int userfd);
        void    	delete_user(int userfd);
        int     	run();
        void    	exec(Token *tok, IrcUser &user);
        int     	used_nickname(std::string str);
        IrcChannel	*is_channel(std::string channel);
        void		kick(Token *t, IrcUser &user);
		void		nickname(Token *t, IrcUser &user, IrcServ &serv);
		int			find_userfd(std::string nick);
};

#endif