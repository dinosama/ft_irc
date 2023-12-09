/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:01:30 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/09 06:54:33 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCUSER_HPP
#define IRCUSER_HPP

#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "Token.hpp"
#include <cstdlib>

class IrcUser
{
    private:
        std::string			name;
        std::string			nick;
        std::string			real_name;
		std::string			pwd;
		std::vector<std::string> send_list;
		bool				op;
		bool				inv;
        int         		userfd;
		int					registered;
        char        		buf[512];
        char        		msg[512];
    public:
        IrcUser(int userfd);
        ~IrcUser();
        char    *getBuf();
        char    *getMsg();
		int		getReg();
		int		getFd();
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
		std::string			getFdstr();
		std::vector<std::string>	*getList();
		bool			getinv();
		void		setinv(bool n);
		bool			getop();
		void		setop(bool n);
        std::string     getPwd();
        void            setPwd(std::string p);
};
#endif