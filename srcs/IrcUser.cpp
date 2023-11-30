/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:24:17 by aaapatou          #+#    #+#             */
/*   Updated: 2023/11/30 04:19:32 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUser.hpp"

IrcUser::IrcUser(int userfd) : userfd(userfd) {
    clearBuf();
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

void    IrcUser::clearBuf()
{
    memset(this->buf, 0, 512);
}

void    IrcUser::clearMsg()
{
    memset(msg, 0, 512);
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
    clearMsg();
    for (int i = 0; i < 512; i++)
    {
        if (buf[i] == '\n' && buf[i - 1] == '\r')
        {
            strncpy(msg, buf, i + 1);
            l = ft_strlen(buf + i + 1, i);
            strncpy(buf, buf + i, l);
            while (l < 512)
            {
                buf[l] = 0;
                l++;
            }
            return (1);
        }
    }
    return (0);
}