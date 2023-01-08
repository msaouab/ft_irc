
#ifndef _CLIENT_HPP__
# define _CLIENT_HPP__

# include "./ircserv.hpp"

class Client
{
	private:
		std::string	user;
	public:
		Client();
		~Client();

		void	setUser(std::string _user);
		std::string	getUser() const ;

};

#endif /* _CLIENT_HPP__ */
