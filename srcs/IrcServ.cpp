/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 04:48:31 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/07 05:01:10 by aaapatou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

int fd_is_valid(int fd)
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void	IrcServ::add_user(int userfd)
{
	struct pollfd pfd;
	pfd.fd = userfd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	users.push_back(IrcUser(userfd));
	pfds.push_back(pfd);
}

void	IrcServ::delete_user(int userfd)
{
	for (int i = 0; i < (int)pfds.size(); i++)
	{
		if (pfds[i].fd == userfd)
		{
			pfds.erase(pfds.begin() + i);
			users.erase(users.begin() + i);
			return ;
		}
	}
}

IrcServ::IrcServ(std::string port, std::string pwd) : pwd(pwd)
{
   	int					status;
   	struct addrinfo		hints;
   	struct addrinfo		*addrlst, *servinfo;
	int					yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port.c_str(), &hints, &addrlst)) != 0) {
		std::cerr << gai_strerror(status) << std::endl;
		exit(status);
	}

	for (servinfo = addrlst; servinfo != NULL; servinfo = servinfo->ai_next) {
		this->sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
		if (sockfd < 0) {
			std::cerr << "error: socket " << sockfd << std::endl;
			continue;
		}
		if ((status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))) {
			std::cerr << "error: setsockopt " << status << std::endl;
			close(sockfd);
			continue;
		}
		if ((status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) < 0) {
			std::cerr << "error: bind " << status << std::endl;
			close(sockfd);
			continue;
		}
		break;
	}

	if (servinfo == NULL) {
		error = 1;
		std::cerr << "error: couldn't bind"<< std::endl;
		close(sockfd);
		return ;
	}
	freeaddrinfo(addrlst);
	add_user(sockfd);
}

IrcServ::~IrcServ()
{
	close(sockfd);
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int		command_is(Token *tok, const char *cmd)
{
	if (tok->getCommand().compare(cmd) == 0)
		return (1);
	return (0);
}

void	IrcServ::exec(Token *tok, IrcUser &user)
{
	std::string		command = tok->getCommand();
	user.getFd();
	if (command.compare("NICK") == 0) {nickname(tok, user, *this); return ;}
	if (command.compare("USER") == 0) {std::cout << "executing USER" << std::endl; return ;}
	if (command.compare("JOIN") == 0) {std::cout << "executing JOIN" << std::endl; return ;}
	if (command.compare("KICK") == 0) {std::cout << "executing KICK" << std::endl; return ;}
	if (command.compare("INVITE") == 0) {std::cout << "executing INVITE" << std::endl; return ;}
	if (command.compare("TOPIC") == 0) {std::cout << "executing TOPIC" << std::endl; return ;}
	if (command.compare("MODE") == 0) {std::cout << "executing MODE" << std::endl; return ;}
	std::cout << "ignoring command " << command << std::endl << std::endl;
	return ;
}

int		IrcServ::run()
{
	int 					status;
	int 					newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t 				addrlen = sizeof remoteaddr;
	char					buf[512];
	int						bufsize = sizeof buf;
	Token					*tok;

	newfd = -1;
	if ((status = listen(sockfd, 10)) == -1) {
		std::cerr << "error: listen " << errno << std::endl;
		return (-1);
	}
	while (power) {
		if ((status = poll(pfds.data(), users.size(), -1)) == -1) {
			std::cerr << "error: poll " << status << std::endl;
			return (-1);
		}
		for (int i = 0; i < (int)users.size(); i++) {
			int nbytes;
			if (pfds[i].revents & POLLIN) {
				if (pfds[i].fd == sockfd) {
					if ((newfd = accept(sockfd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
						std::cerr << "error: accept" << std::endl;
					} else {
						add_user(newfd);
						std::cerr << "server: new connection on socket " << newfd << std::endl;
					}
					std::cerr << "new fd is " << newfd << std::endl;
				} else {
					std::cout << "receive something on socket " << pfds[i].fd << " users length " << users.size() << std::endl;
					memset(buf, 0, 512);
					nbytes = recv(pfds[i].fd, buf, bufsize, 0);
					if (nbytes < 0)
						std::cerr << "error: recv " << nbytes << std::endl;
					else if (nbytes == 0)
					{
						std::cout << "connection with user " << pfds[i].fd << " is closed" << std::endl;
						delete_user(pfds[i].fd);
					}
					else
					{
						if ((strlen(buf) + strlen(users[i].getBuf()) > 512))
						{
							std::cerr << "error: buffer overflow" << std::endl;
							std::cerr << "server: closing clientfd " << pfds[i].fd << std::endl;
							delete_user(pfds[i].fd);
						}
						users[i].buffing(buf);
						while (users[i].buftomsg())
						{
							tok = new Token(users[i].getMsg());
							std::cout << "command: " << tok->getCommand() << std::endl;
							for (int i = 0; i < (int)tok->getNparam(); i++)
								std::cout << "parameter " << i + 1 << ": " << (*tok->getParam())[i] << std::endl;
							exec(tok, users[i]);
							std::cout << std::endl;
							delete tok;
						}
					}
				}
			}
		}
	}
	return (1);
}