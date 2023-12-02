/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:01:30 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/02 02:27:19 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCUSER_HPP
#define IRCUSER_HPP

#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

class IrcUser
{
    private:
        std::string name;
        std::string nick;
        std::string real_name;
        int         userfd;
        char        buf[512];
        char        msg[512];
    public:
        IrcUser(int userfd);
        ~IrcUser();
        char    *getBuf();
        char    *getMsg();
		int		getFd();
        void    clearBuf();
        void    clearMsg();
        int     buftomsg();
        void    buffing(char *s);
};

#endif