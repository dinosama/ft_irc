/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 04:30:53 by aaapatou          #+#    #+#             */
/*   Updated: 2023/11/30 05:01:02 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"
#include <cstring>

Token::Token(std::string &msg)
{
	int nparam = 0;
	std::string str;

	command.assign(msg.data(), 0, msg.find_first_of(' '));
	for (int i = (int)msg.find_first_of(' ') + 1; i < msg.size(); i++)
	{
		int start = i;
		while (msg[i] != '\0' && msg[i] != ' ')
		{
			i++;
		}
		parameters.push_back(str.assign(msg.data(), start, i - start));
		str.clear();
	}
}