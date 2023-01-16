
#include "../Includes/server.hpp"

server::server() {
}

server::server(int _port, std::string _pswd) {
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
	std::cout << GREEN << "\n\t\tWELCOME TO OUR_IRC SERVER" << ED << std::endl << std::endl;
	std::cout << GRAY << "To connect with our server please enter: " << std::endl;
	std::cout << "Command: `PASS ` password server" << std::endl;
	std::cout << "Command: `NICK ` Your nickname in server please shoose one not unique" << std::endl;
	std::cout << "Command: `USER ` Your username in the server " << ED << std::endl;
	return (n_fds);
}

void	server::Check_pass(std::string pass, std::string password)
{
	if(pass.compare(5, password.length(), password))
    {
        std::cout << RED << "\nWRONG PASSWORD!!!\n" << ED << std::endl;
		return ;
    }
}

void	server::Check_nick(std::string nick)
{
	nick = nick.substr(5, nick.length());
	std::pair<std::map<std::string, Client>::iterator,bool> ret;
	ret = myGuest.insert(std::pair<std::string, Client>(nick, Client(i)));
	std::cout << "this fdsocket ==> " << fds[i].fd << " i ==> " << i << std::endl;
	if (ret.second == false)
		std::cout << "this nickname already exist" << std::endl;
	else
		setNick(nick);
}

void	server::Check_user(std::string user)
{
	// char	**userArr;
	user = user.substr(5, user.length());
	// userArr = ft_split(user.c_str(), ' ');
	myGuest[nick].setUser(user);
	std::map<std::string, Client>::iterator it = myGuest.begin();
	for (it=myGuest.begin(); it!=myGuest.end(); ++it)
		std::cout << it->first << " => " << it->second.getUser() << '\n';
}

void	server::Parse_cmd(std::string input)
{
	if (!input.compare(0, 4, "PASS"))
		Check_pass(input, password);
	else if (!(input.compare(0, 4, "NICK")))
		Check_nick(input);
	else if (!(input.compare(0, 4, "USER")))
		Check_user(input);
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
	// if (input[0] == buffer[setsock - 1])
	// 	return (false);
	// cid = fds[i].fd;
	Parse_cmd(input);
	return (true);
}

// void	server::sendMessage()
// {
// 	int	len;

// 	len = setsock;
// 	// setsock = send(fds[i].fd, buffer, len, 0);
// 	// if (setsock < 0) {
// 	// 	std::cout << "send() failed\n" << std::endl;
// 	// 	st_conx = true;
// 	// 	break ;
// 	// }
// }

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
		for (i = 0; i < MAX_CLIENT; i++) {
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
					// sendMessage();
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