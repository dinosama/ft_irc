/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 01:04:58 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/02 02:27:51 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <vector>

class Token
{
	private:
	std::string msg;
	public:
	Token(char *cmsg);
	~Token();
	std::string	command;
	std::vector<std::string> parameters;
	int		nparam;
};

#endif