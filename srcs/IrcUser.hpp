/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motaouss <motaouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:01:30 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/06 22:46:59 by aaapatou         ###   ########.fr       */
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
		int			registered;
        char        buf[512];
        char        msg[512];
    public:
        IrcUser(int userfd);
        ~IrcUser();
        char    *getBuf();
        char    *getMsg();
		int		getReg();
		int		getFd();
        std::string getNick();
        std::string getName();
        void    clearBuf();
        void    clearMsg();
        int     buftomsg();
        void    buffing(char *s);
        std::string getNick();
        std::string getName();
		void		setNick(std::string nick);
		void		setName(std::string name);
		void		setReal(std::string real);
		void		setReg(int reg);
};

#endif