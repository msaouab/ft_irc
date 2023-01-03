
#ifndef _SERVER_HPP__
# define _SERVER_HPP__

#include "./ircserv.hpp"

// struct sockaddr_in	address;

class server
{
	private:
		int			port;
		int			sock_fd;
		int			setsock;
		int			addrLen;
		int			timeout;
		std::string	password;
		std::string	buffer[80];
		bool		End_server;
	public:
		server();
		server(int _port, std::string _pswd);
		~server();
		int			getPort() const;
		std::string	getPassword() const;
		void		setPort(int port);
		void		setPassword(std::string password);
		void		start();
		class ErrorPortException : public std::exception
		{
			const char *what() const throw();
		};
};

#endif /* _SERVER_HPP__ */
