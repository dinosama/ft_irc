/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 04:48:38 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/02 02:39:39 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return (0);
    std::string pwd(av[2]);
    std::string port(av[1]);
    IrcServ serv(port, pwd);
    serv.run();
    return (1);
}