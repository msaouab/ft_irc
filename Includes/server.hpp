
#ifndef _SERVER_HPP__
# define _SERVER_HPP__

#include "./ircserv.hpp"

# define MAX_CLIENT 17
# define TIMEOUT (10 * 60 * 1000)

class server
{
	private:
		int			port;
		int			sock_fd;
		int			setsock;
		int			timeout;
		std::string	password;
		bool		End_server;
		bool		st_conx;
		int			n_fds;
	public:
		
		struct sockaddr_in	address;
		socklen_t			addrLen;
		struct pollfd		fds[200];
		server();
		server(int _port, std::string _pswd);
		~server();

		int			getPort() const;
		std::string	getPassword() const;
		void		setPort(int port);
		void		setPassword(std::string password);

		void		start();
		void		CreateSocket();
		void		bindThesocket();
		bool		WaitClient();
		int			acceptSocket(int n_fds);
		bool		recvMessage(char *buffer, int i);
		void		sendMessage();

		class ErrorPortException : public std::exception
		{
			const char *what() const throw();
		};
};

#endif /* _SERVER_HPP__ */
