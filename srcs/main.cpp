/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 04:48:38 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/07 05:00:26 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"
#include <signal.h>

#define ON 1
#define OFF 0

bool	power = ON;

static void	signal_handler(int signal)
{
	(void)signal;
	power = OFF;
}

int main(int ac, char **av)
{
	signal(SIGINT, signal_handler);
    if (ac != 3)
        return (0);
    std::string pwd(av[2]);
    std::string port(av[1]);
    IrcServ serv(port, pwd);
    serv.run();
    return (1);
}