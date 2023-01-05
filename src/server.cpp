
#include "../Includes/server.hpp"

server::server() {
}

server::server(int _port, std::string _pswd) {
	this->port = _port;
	this->password = _pswd;
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

void	server::start()
{
	struct sockaddr_in	address;
	struct pollfd		fds[200];
	int					opt;
	int					n_fds;
	int					current_size;
	int					new_sd;
	int					len;
	bool				close_conn;
	bool				compress_arr;
	socklen_t			addrLen;
	char				buffer[1024];

	opt = 1;
	n_fds = 1;
	current_size = 0;
	new_sd = -1;
	End_server = false;
	compress_arr = false;
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
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	// memset(&address, 0, sizeof(address));
	setsock = bind(sock_fd, (struct sockaddr *)&address, sizeof(address));
	if (setsock < 0) {
		std::cout << "bind() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
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
	/*************************************************************/
	/* Initialize the timeout to 10 minutes. If no               */
	/* activity after 10 minutes this program will end.          */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = (10 * 60 * 1000);
	while (!End_server) {
		/***********************************************************/
		/* Call poll() and wait 10 minutes for it to complete.     */
		/***********************************************************/
		std::cout << "Waiting on poll()...\n" << std::endl;
		setsock = poll(fds, n_fds, timeout);
		if (setsock < 0) {
			std::cout << "poll() failed: " << strerror(errno) << '\n';
			break ;
		}
		if (setsock == 0) {
			std::cout << GRAY << "poll() timeout. End Program.\n" << ED << std::endl;
			break ;
		}
		current_size = n_fds;
		for (int i = 0; i < current_size; i++) {
			if (fds[i].revents == 0)
				continue ;
			if (fds[i].revents != POLLIN) {
				std::cout << "Error! revents ==> " << fds[i].revents << '\n' << std::endl;
				End_server = true;
				break ;
			}
			if (fds[i].fd == sock_fd) {
				std::cout << "Listening Socker is readable\n" << std::endl;
				do {
					new_sd = accept(sock_fd, (struct sockaddr *)&address, &addrLen);
					if (new_sd < 0) {
						if (errno != EWOULDBLOCK) {
							std::cout << "accept() failed: " << strerror(errno) << '\n'<< std::endl;
							End_server = true;
						}
						break ;
					}
					/*****************************************************/
					/* Add the new incoming connection to the            */
					/* pollfd structure                                  */
					/*****************************************************/
					std::cout << "New incomming connection ==> " << new_sd << std::endl;
					fds[n_fds].fd = new_sd;
					fds[n_fds].events = POLLIN;
					n_fds++;
					/*****************************************************/
					/* Loop back up and accept another incoming          */
					/* connection                                        */
					/*****************************************************/
				} while (new_sd != -1);
			}
			else {
					// std::cout  << "setsocket : " << setsock << std::endl;
				std::cout << "Discriptor " << fds[i].fd << " is readable\n" << std::endl;
				close_conn = false;
				do {
					setsock = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					// std::cout  << "setsocket : " << setsock << std::endl;
					buffer[setsock] = '\0';
					std::cout  << "buffer.size() : " << strlen(buffer) << std::endl;
					std::cout  << "recieved : " << buffer;
					if (setsock < 0) {
						if (errno != EWOULDBLOCK) {
							std::cout << "recv() failed " << strerror(errno) << '\n' << std::endl;
							close_conn = true;
						}
						break ;
					}
					/*****************************************************/
					/* Check to see if the connection has been           */
					/* closed by the client                              */
					/*****************************************************/
					if (setsock == 0) {
						std::cout << "Connection closed\n" << std::endl;;
						close_conn = true;
						break ;
					}
					/*****************************************************/
					/* Data was received                                 */
					/*****************************************************/
					len = setsock;
					std::cout << len << " Bytes received\n" << std::endl;
					/*****************************************************/
					/* Echo the data back to the client                  */
					/*****************************************************/
					// setsock = send(fds[i].fd, buffer, len, 0);
					// if (setsock < 0) {
					// 	std::cout << "send() failed\n" << std::endl;
					// 	close_conn = true;
					// 	break ;
					// }
				} while (true);
				if (close_conn) {
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_arr = true;
				}
			}
		}
		// if (compress_arr) {
		// 	for (int i = 0; i < n_fds; i++) {
		// 		if (fds[i].fd == -1) {
		// 			for (int j = i; j < n_fds - 1; j++)
		// 				fds[j].fd = fds[j + 1].fd;
		// 			i--;
		// 			n_fds--;
		// 		}
		// 	}
		// }
	}

}

const char* server::ErrorPortException::what() const throw()
{
	return("\033[1;31m Error in Port number \033[0m");
}
