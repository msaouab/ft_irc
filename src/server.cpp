
#include "../Includes/server.hpp"
#include "../Includes/Bot.hpp"
#include<sstream>  
#include <iostream>  
server::server() {
}

server::server(int _port, std::string _pswd) {
	if (_port < 6665 || _port > 6669)
		throw server::ErrorPortException();
	this->port = _port;
	this->password = _pswd;
	this->n_fds = 1;
}

server::~server() {
}

int server::getPort() const
{
    return (this->port);
}

std::string	server::getPassword() const
{
    return (this->password);
}

void	server::setPort(int port)
{
    this->port = port;
}

void	server::setPassword(std::string password)
{
    this->password = password;
}
void	server::setNick(std::string _nick) {
	this->nick = _nick;
}

std::string		server::getNick() const {
	return (this->nick);
}

void	server::CreateSocket()
{
	int	opt;

	opt = 1;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		std::cout << "Socket() failed: " << strerror(errno) << '\n';
		exit(EXIT_FAILURE);
	}
	rc = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (rc < 0) {
		std::cout << "setsockopt() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	rc = fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	if (rc < 0) {
		std::cout << "fcntl() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
}

void	server::bindThesocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	rc = bind(sock_fd, (struct sockaddr *)&address, sizeof(address));
	if (rc < 0) {
		std::cout << "bind() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	rc = listen(sock_fd, 3);
	if (rc < 0) {
		std::cout << "listen() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	addrLen = sizeof(address);
	memset(fds, 0, sizeof(fds));
	std::cout << "Looking For Connections" << std::endl;
	fds[0].fd = sock_fd;
	fds[0].events = POLLIN;
}

bool	server::WaitClient()
{
	rc = poll(fds, n_fds, TIMEOUT);
	if (rc < 0) {
		std::cout << "poll() failed: " << strerror(errno) << '\n';
		return (false) ;
	}
	if (rc == 0) {
		std::cout << GRAY << "poll() timeout. End Program.\n" << ED << std::endl;
		return (false) ;
	}
	return (true);
}

int	server::acceptSocket(int n_fds)
{
	int	new_sd;
	std::string	ip_addr;
	struct timeval tv;
	time_t	time;

	new_sd = -1;
	while (new_sd == -1) {
		new_sd = accept(sock_fd, (struct sockaddr *)&address, &addrLen);
		if (new_sd < 0) {
			if (errno != EWOULDBLOCK) {
				std::cout << "accept() failed: " << strerror(errno) << '\n'<< std::endl;
				End_server = true;
			}
			break ;
		}
		gettimeofday(&tv, NULL);
		time = tv.tv_sec;
		myGuest[new_sd].setTime(time);
		std::cout << "New incomming connection ==> " << new_sd << std::endl;
		ip_addr = inet_ntoa(address.sin_addr);
		myGuest[new_sd].setIP(ip_addr);
		fds[n_fds].fd = new_sd;
		fds[n_fds].events = POLLIN;
		n_fds++;
	}
	return (n_fds);
}


bool	server::recvMessage(int i)
{
	char		buffer[DEFAULT_BUFLEN];
	std::string	input;
	
	rc = recv(fds[i].fd, buffer, DEFAULT_BUFLEN, 0);
	if (rc < 0) {
		if (errno != EWOULDBLOCK) {
			myGuest.erase(fds[i].fd);
			std::cout << "recv() failed: " << strerror(errno) << std::endl;
			st_conx = true;
		}
		return (false);
	}
	if (rc == 0) {
		std::cout << "Connection closed" << std::endl;
		Check_quit(i);
		st_conx = true;
		return (false);
	}
	buffer[rc] = '\0';
	if (rc != 1)
		input = strtok(buffer, "\r\n");
	Parse_cmd(input, i);
	return (true);
}

void	server::start()
{
	int					current_size;
	bool				compress_arr;
	int					i = 0;

	n_fds = 1;
	current_size = 0;
	End_server = false;
	compress_arr = false;
	CreateSocket();
	bindThesocket();
	while (!End_server) {
		if (WaitClient() == false)
			break ;
		current_size = n_fds;
		for (i = 0; i < MAX_CLIENT; i++) {
			if (fds[i].revents == 0)
				continue ;
			if (fds[i].fd == sock_fd) {
				n_fds = acceptSocket(n_fds);
			}
			else {
				std::cout << "Discriptor " << fds[i].fd << " is readable\n" << std::endl;
				st_conx = false;
				recvMessage(i);
				while (true) {
					if (recvMessage(i) == false)
						break ;
				}
				if (st_conx) {
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_arr = true;
				}
			}
		}
	}
}

const char* server::ErrorPortException::what() const throw()
{
	return("\033[1;31m Error in Port number. The range is 6665-6669 \033[0m");
}