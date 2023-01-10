
#include "../Includes/server.hpp"

size_t retry = 0;

server::server() {
}

server::server(int _port, std::string _pswd) {
	this->port = _port;
	this->password = _pswd;
	this->n_fds = 1;
	this->logged = false;
}

server::~server() {
}

int server::getPort() const
{
    return (this->port);
}

std::string server::getPassword() const
{
    return (this->password);
}

void server::setPort(int port)
{
    this->port = port;
}

void server::setPassword(std::string password)
{
    this->password = password;
}

void	server::CreateSocket()
{
	int					opt;

	opt = 1;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		std::cout << "Socket() failed: " << strerror(errno) << '\n';
		exit(EXIT_FAILURE);
	}
	setsock = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (setsock < 0) {
		std::cout << "setsockopt() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	setsock = fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	if (setsock < 0) {
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
	setsock = bind(sock_fd, (struct sockaddr *)&address, sizeof(address));
	if (setsock < 0) {
		std::cout << "bind() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	setsock = listen(sock_fd, 3);
	if (setsock < 0) {
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
	std::cout << "Waiting on poll()...\n" << std::endl;
	setsock = poll(fds, n_fds, TIMEOUT);
	if (setsock < 0) {
		std::cout << "poll() failed: " << strerror(errno) << '\n';
		return (false) ;
	}
	if (setsock == 0) {
		std::cout << GRAY << "poll() timeout. End Program.\n" << ED << std::endl;
		return (false) ;
	}
	return (true);
}

int	server::acceptSocket(int n_fds)
{
	int	new_sd;

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
		std::cout << "New incomming connection ==> " << new_sd << std::endl;
		fds[n_fds].fd = new_sd;
		fds[n_fds].events = POLLIN;
		n_fds++;
	}
	return (n_fds);
}

void	server::Check_pass(std::string pass, std::string password)
{
	if((pass.length() < 6 || pass.compare(5, password.length(), password)) && !logged)
    {
        std::cout << RED << "\nWRONG PASSWORD!!!\n" << ED << std::endl;
			retry++;
        std::cout << RED << retry << ED << std::endl;
		if(retry > 2)
		{
			std::cout << RED << "\nYOU HAVE PASSED 3 TRIES , BYE!!!\n" << ED << std::endl;
			exit(EXIT_FAILURE);
		}
		else 
			return;
    }
    else
    {
		if (logged == true)
		{	
			std::cout << RED << "\nWARNING! YOU ARE ALREADY LOGGED IN!!\n" << ED << std::endl;
			return;
		}
		else
		{
			std::cout << GREEN << "\nWELCOME TO OUR_IRC SERVER\n" << ED << std::endl;
			logged = true;
        	return;
		}
    }
}

void	server::Check_nick(std::string nick)
{
	std::pair<std::map<std::string, int>::iterator,bool> ret;

	nick = nick.substr(5, nick.length());
	ret = myClient.insert(std::pair<std::string, int>(nick, client));
	if (!ret.second)
		std::cout << "Your Nickname not Insterted please try again" << std::endl;
	// std::map<std::string, int>::iterator it = myClient.begin();
	// std::cout << nick << ' ' << client << '\n';
	// for (it=myClient.begin(); it!=myClient.end(); it++) {
	// 	std::cout << it->first << " => " << it->second << '\n';
	// }
}

// void	server::Check_user(std::string user)
// {
// 	nick = nick.substr(5, nick.length());

// }

void	server::Parse_cmd(std::string input)
{
	if (!input.compare(0, 4, "PASS"))
		Check_pass(input, password);
	else if (!(input.compare(0, 4, "NICK")))
		Check_nick(input);
	// else if (!(input.compare(0, 4, "USER")))
	// 	Check_user(std::string input);
	else
		std::cout << RED << "\nCommand not found\n" << ED << std::endl;
}


bool	server::recvMessage(int i)
{
	char		buffer[DEFAULT_BUFLEN];
	std::string	input;

	setsock = recv(fds[i].fd, buffer, DEFAULT_BUFLEN, 0);
	if (setsock < 0) {
		if (errno != EWOULDBLOCK) {
			std::cout << "recv() failed " << strerror(errno) << '\n' << std::endl;
			st_conx = true;
		}
		return (false);
	}
	if (setsock == 0) {
		std::cout << "Connection closed\n" << std::endl;
		st_conx = true;
		return (false);
	}
	buffer[setsock] = '\0';
	input = strtok(buffer, "\r\n");
	client = fds[i].fd - 3;
	Parse_cmd(input);
	return (true);
}

void	server::sendMessage()
{
	int	len;

	len = setsock;
	// setsock = send(fds[i].fd, buffer, len, 0);
	// if (setsock < 0) {
	// 	std::cout << "send() failed\n" << std::endl;
	// 	st_conx = true;
	// 	break ;
	// }
}

void	server::start()
{
	
	int					current_size;
	bool				compress_arr;

	current_size = 0;
	End_server = false;
	compress_arr = false;
	CreateSocket();
	bindThesocket();
	while (!End_server) {
		if (WaitClient() == false)
			break ;
		current_size = n_fds;
		for (int i = 0; i < MAX_CLIENT; i++) {
			if (fds[i].revents == 0)
				continue ;
			if (fds[i].fd == sock_fd) {
				// std::cout << "Listening Socker is readable\n" << std::endl;
				// std::cout << "client ==> " << i + 1 << '\n';
				n_fds = acceptSocket(n_fds);
			}
			else {
				// std::cout << "Discriptor " << fds[i].fd << " is readable\n" << std::endl;
				st_conx = false;
				while (true) {
					if (recvMessage(i) == false)
						break ;
					sendMessage();
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
	return("\033[1;31m Error in Port number \033[0m");
}
