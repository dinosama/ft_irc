/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 04:30:53 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/06 03:28:10 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"
#include <cstring>

Token::Token(char *cmsg) : msg(cmsg)
{
	nparam = 0;
	std::string str;

	command.assign(msg.data(), 0, msg.find_first_of(' '));
	for (int i = (int)msg.find_first_of(' ') + 1; i < (int)msg.size(); i++)
	{
		int start = i;
		if (msg[start] != ':')
		{
			while (msg[i] != '\0' && msg[i] != ' ')
			{
				i++;
			}
		}
		else
		{
			start++;
			while (msg[i] != '\0')
			{
				i++;
			}
		}
		parameters.push_back(str.assign(msg.data(), start, i - start));
		nparam++;
		str.clear();
	}
}

Token::~Token() {}

std::string		Token::getCommand() const
{
	return (command);
}

std::string		Token::getMsg() const
{
	return (msg);
}

std::vector<std::string>	*Token::getParam()
{
	return (&parameters);
}

int							Token::getNparam() const
{
	return (nparam);
}
