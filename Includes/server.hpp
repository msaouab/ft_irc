
#ifndef _SERVER_HPP__
# define _SERVER_HPP__

#include "./ircserv.hpp"

class server
{
	private:
	int port;
	std::string password;
	public:
		server();
		~server();
		int getPort() const;
		std::string getPassword() const;
		void setPort(int port);
		void setPassword(std::string password);
};

#endif /* _SERVER_HPP__ */