/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 01:04:58 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/06 03:21:17 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <vector>

class Token
{
	private:
		std::string 				msg;
		std::string					command;
		std::vector<std::string> 	parameters;
		int							nparam;
	public:
		Token(char *cmsg);
		~Token();
		std::string	getMsg() const;
		std::string getCommand() const;
		std::vector<std::string> *getParam();
		int			getNparam() const;
};

#endif