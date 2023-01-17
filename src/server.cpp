
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
	std::string welcome = _welcomemsg();

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
		if (send(new_sd, welcome.c_str(), welcome.length(), 0) <= 0)
			std::cout << strerror(errno);
		fds[n_fds].fd = new_sd;
		fds[n_fds].events = POLLIN;
		n_fds++;
	}
	return (n_fds);
}

void	server::Check_pass(std::string pass, std::string password, int i)
{
	std::string message;
	message = "Incorrect Password\n";
	if(pass.compare(5, password.length(), password)) {
		myGuest[fds[i].fd].setAuth(false);
		sendError(fds[i].fd, message, RED);
		return ;
    }
	myGuest.insert(std::pair<int, Client>(fds[i].fd, Client()));
	myGuest[fds[i].fd].setAuth(true);
}

void	server::Check_nick(std::string nick, int i)
{
	std::string	message;
	message = "You need to login so you can start chatting OR you can send HELP to see how :)\n";
	// std::cout << myGuest[fds[i].fd].getAuth() << std::endl;
	if (!myGuest[fds[i].fd].getAuth()) {
		sendError(fds[i].fd, message, RED);
		return ;
	}
	message = "this nickname already exist\n";
	nick = nick.substr(5, nick.length());
	std::map<int, Client>::iterator it;
	for (it = myGuest.begin(); it != myGuest.end(); it++) {
		if (it->second.getNick() == nick) {
			sendError(fds[i].fd, message, RED);
			// myGuest[fds[i].fd].setAuth(false);
			return ;
		}
	}
	myGuest[fds[i].fd].setNick(nick);
	// std::cout << myGuest[fds[i].fd].getClientfd() << " ==> " << myGuest[fds[i].fd].getNick() << std::endl;
	myGuest[fds[i].fd].setAuth(true);
}

void	server::Check_user(std::string user, int i)
{
	std::string	message;
	message = "You need to login so you can start chatting OR you can send HELP to see how :)\n";
	std::cout << myGuest[fds[i].fd].getAuth() << std::endl;
	if (!myGuest[fds[i].fd].getAuth()) {
		sendError(fds[i].fd, message, RED);
		return ;
	}
	char	**userArr;
	user = user.substr(5, user.length());
	userArr = ft_split(user.c_str(), ' ');
	if (lenArr(userArr) != 4) {
		message = "Command: USER.\nParameters: <username> <hostname> <servername> <realname>.\n";
		sendError(fds[i].fd, message, RED);
		return ;
	}
	myGuest[fds[i].fd].setUser(userArr);
}

void	server::Check_quit(int i)
{
	std::string	message;
	message = "You are leaving the server.\nsee you later :)";
	sendError(fds[i].fd, message, GREEN);
	myGuest.erase(fds[i].fd);
	std::map<int, Client>::iterator it;
	for (it = myGuest.begin(); it != myGuest.end(); it++) {
		std::cout << it->second.getClientfd() << " ==> " << it->second.getNick() << std::endl;
	}
	close(fds[i].fd);
}

void	server::Parse_cmd(std::string input, int i)
{
	std::string	message;
	message = "Command not found\n";
	if (!input.compare(0, 4, "PASS"))
		Check_pass(input, password, i);
	else if (!(input.compare(0, 4, "NICK")))
		Check_nick(input, i);
	else if (!(input.compare(0, 4, "USER")))
		Check_user(input, i);
	else if (!(input.compare(0, 4, "QUIT")))
		Check_quit(i);
	else
		sendError(fds[i].fd, message, RED);
		// if (send(fds[i].fd, message.c_str(), message.length(), 0) >= 0)
			// perror(strerror(errno));
}

bool	server::recvMessage(int i)
{
	char		buffer[DEFAULT_BUFLEN];
	std::string	input;

	rc = recv(fds[i].fd, buffer, DEFAULT_BUFLEN, 0);
	if (rc < 0) {
		if (errno != EWOULDBLOCK) {
			std::cout << "recv() failed: " << strerror(errno) << std::endl;
			st_conx = true;
		}
		return (false);
	}
	if (rc == 0) {
		std::cout << "Connection closed" << std::endl;
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
	int					i;

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
				std::cout << "Listening Socker is readable\n" << std::endl;
				n_fds = acceptSocket(n_fds);
			}
			else {
				std::cout << "Discriptor " << fds[i].fd << " is readable\n" << std::endl;
				st_conx = false;
				recvMessage(i);
				// while (true) {
				// 	if (recvMessage(i) == false)
				// 		break ;
				// 	// sendMessage();
				// }
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
	return("\033[1;31m Error in Port number \033[0m");
}