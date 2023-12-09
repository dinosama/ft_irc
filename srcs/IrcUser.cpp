/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:24:17 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/09 09:21:26 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUser.hpp"

IrcUser::IrcUser(int userfd) : userfd(userfd) {
    op = 0;
    inv = 0;
	registered = 0;
    clearMsg();
	clearBuf();
}

IrcUser::~IrcUser() {
}

char    *IrcUser::getBuf()
{
    return (buf);
}

char    *IrcUser::getMsg()
{
    return (msg);
}

int		IrcUser::getFd()
{
	return (userfd);
}

std::string IrcUser::getNick(){ return (nick); }

std::string IrcUser::getName(){ return (name); }

void    IrcUser::clearBuf()
{
    memset(this->buf, 0, 512);
}

void    IrcUser::clearMsg()
{
    memset(this->msg, 0, 512);
}

int     ft_strlen(char *s, int i)
{
    int l = 0;
    
    while (s[l] && (l + i) < 512)
        l++;
    return (l);
}

void    IrcUser::buffing(char *s)
{
    strcat(buf, s);
}

int     IrcUser::buftomsg()
{
    int l;
	char	tmp[512];

	memset(tmp, 0, 512);
    clearMsg();
    for (int i = 0; i < 512; i++)
    {
        if (buf[i] == '\n' && buf[i - 1] == '\r')
        {
            strncpy(msg, buf, i - 1);
            l = ft_strlen(buf + i, i);
			if (l > 0)
			{
            	strncpy(tmp, buf + i + 1, l);
				strcpy(buf, tmp);
			}
            while (l + 1 < 512)
            {
                buf[l + 1] = 0;
                l++;
            }
            return (1);
        }
    }
    return (0);
}

void	IrcUser::setNick(std::string nick)
{
	this->nick = nick;
}

void	IrcUser::setName(std::string name)
{
	this->name = name;
}

void	IrcUser::setReal(std::string real)
{
	this->real_name = real;
}

void	IrcUser::setRegplus()
{
	registered++;
}

int		IrcUser::getReg()
{
	return (registered);
}

std::string     ft_itoa(int nb)
{
    std::string dest;

    if (nb > 9)
    {
        dest = ft_itoa(nb / 10);
    }
    dest += (nb % 10) + 48;
    return dest;
}

std::string             IrcUser::getFdstr()
{
    std::string str;

    str = ft_itoa(userfd);
    return (str);
}

std::vector<std::string>    *IrcUser::getList()
{
    return (&send_list);
}

bool     IrcUser::getop()
{
    return (op);
}

void    IrcUser::setop(bool n)
{
    op = n;
}

bool     IrcUser::getinv()
{
    return (inv);
}

void    IrcUser::setinv(bool n)
{
    inv = n;
}

std::string             IrcUser::getPwd()
{
    return (pwd);
}

void                    IrcUser::setPwd(std::string p)
{
    pwd = p;
}