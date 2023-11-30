/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 01:04:58 by aaapatou          #+#    #+#             */
/*   Updated: 2023/11/30 04:37:59 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>

class Token
{
	private:
	public:
	Token(std::string &msg);
	~Token();
	std::string	command;
	std::vector<std::string> parameters;
};