/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:01:30 by aaapatou          #+#    #+#             */
/*   Updated: 2023/11/26 10:14:47 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

class IrcUser
{
    private:
        int     userfd;
        char    buf[512];
        char    msg[512];
    public:
        IrcUser(int userfd);
        ~IrcUser();
        char    *getBuf();
        char    *getMsg();
        void    clearBuf();
        void    clearMsg();
        int     buftomsg();
        void    buffing(char *s);
};