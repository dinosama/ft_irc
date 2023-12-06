/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:24:17 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/06 04:47:04 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcUser.hpp"

IrcUser::IrcUser(int userfd) : userfd(userfd) {
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