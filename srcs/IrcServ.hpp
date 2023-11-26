/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 04:48:27 by aaapatou          #+#    #+#             */
/*   Updated: 2023/11/26 08:47:25 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

class IrcServ
{
    private:
        int                             sockfd;
        int                             error;
        std::vector<IrcUser>            userdb;
        std::vector<pollfd>             pfds;
    public:
        IrcServ();
        ~IrcServ();
        void    add_user(int userfd);
        void    delete_user(int userfd);
        int     run();
};