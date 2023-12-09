/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaapatou <aaapatou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 04:48:31 by aaapatou          #+#    #+#             */
/*   Updated: 2023/12/09 07:27:17 by aaapatou         ###   ########.fr       */
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
	fcntl(userfd, F_SETFL, O_NONBLOCK);
	pfd.fd = userfd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	users.push_back(IrcUser(userfd));
	pfds.push_back(pfd);
}

void	IrcServ::delete_user(int userfd)
{
	std::string nick;
	for (int i = 0; i < (int)pfds.size(); i++)
	{
		if (pfds[i].fd == userfd)
		{
			pfds.erase(pfds.begin() + i);
			nick = (*(users.begin() + i)).getNick();
			users.erase(users.begin() + i);
			return ;
		}
	}
	for (std::vector<IrcChannel>::iterator it = channels.begin(); it < channels.end(); it++)
	{
		for (std::vector<IrcUser>::iterator it1 = (*it).getUsers()->begin(); it1 < (*it).getUsers()->end(); it1++)
		{
			if ((*it1).getNick() == nick)
				(*it).getUsers()->erase(it1);
		}
		for (std::vector<IrcUser>::iterator it1 = (*it).getOps()->begin(); it1 < (*it).getOps()->end(); it1++)
		{
			if ((*it1).getNick() == nick)
				(*it).getUsers()->erase(it1);
		}
	}
}

void	IrcServ::add_channel(std::string title, IrcUser user)
{
	IrcChannel		chan(title, user);

	chan.addOp(user);
	channels.push_back(chan);
}

void	IrcServ::delete_channel(std::string title)
{
	for (std::vector<IrcChannel>::iterator it = channels.begin(); it < channels.end(); it++)
	{
		if (title == (*it).getTitle())
			channels.erase(it);
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
	fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if (servinfo == NULL) {
		error = 1;
		std::cerr << "error: couldn't bind"<< std::endl;
		close(sockfd);
		return ;
	}
	if ((status = listen(sockfd, 10)) == -1) {
		std::cerr << "error: listen " << errno << std::endl;
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
	if (command.compare("NICK") == 0) {nickname(tok, user, *this); std::cout << "executing NICK" << std::endl; return ;}
	if (command.compare("PRIVMSG") == 0) {prvmsg(tok, user); std::cout << "executing PRIVMSG" << std::endl; return ;}
	if (command.compare("PING") == 0) {pong(tok, user); std::cout << "executing PONG" << std::endl; return ;}
	if (command.compare("PASS") == 0) {pass(tok, user); std::cout << "executing PASS" << std::endl; return ;}
	if (command.compare("USER") == 0) {user_cmd(tok, user); std::cout << "executing USER" << std::endl; return ;}
	if (command.compare("JOIN") == 0) {join(tok, user); std::cout << "executing JOIN" << std::endl; return ;}
	if (command.compare("KICK") == 0) {kick(tok, user); std::cout << "executing KICK" << std::endl; return ;}
	if (command.compare("INVITE") == 0) {invite(tok, user); std::cout << "executing INVITE" << std::endl; return ;}
	if (command.compare("TOPIC") == 0) {topic(tok, user); std::cout << "executing TOPIC" << std::endl; return ;}
	if (command.compare("MODE") == 0) {mode(tok, user); std::cout << "executing MODE" << std::endl; return ;}
	if (command.compare("OPER") == 0) {oper(tok, user); std::cout << "executing OPER" << std::endl; return ;}
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

	newfd = -1;
	while (power) {
		if ((status = poll(pfds.data(), users.size(), 0)) == -1) {
			std::cerr << "error: poll " << status << std::endl;
			return (-1);
		}
		for (int i = 0; i < (int)users.size(); i++) {
			int nbytes;
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == sockfd)
				{
					if ((newfd = accept(sockfd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1)
						std::cerr << "error: accept" << std::endl;
					else
					{
						add_user(newfd);
						std::cerr << "server: new connection on socket " << newfd << std::endl;
					}
				}
				else
				{
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
							Token tok(users[i].getMsg());
							std::cout << "command: " << tok.getCommand() << std::endl;
							for (int i = 0; i < (int)tok.getNparam(); i++)
								std::cout << "parameter " << i + 1 << ": " << (*tok.getParam())[i] << std::endl;
							std::cout << std::endl;
							exec(&tok, users[i]);
						}
					}
				}
			}
			else if (pfds[i].revents & POLLOUT)
			{
				for (std::vector<std::string>::iterator it = users[i].getList()->begin(); it < users[i].getList()->end(); it++)
				{
					if (send(users[i].getFd(), (*it).c_str(), (*it).size(), 0) == -1)
						std::cerr << "error: send" << std::endl;
				}
				users[i].getList()->clear();
			}
		}
	}
	return (1);
}