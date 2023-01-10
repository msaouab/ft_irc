
#ifndef _CLIENT_HPP__
# define _CLIENT_HPP__

# include "./ircserv.hpp"

class Client
{
	private:
		std::string	pass;
		std::string	nick;
		std::string	user;
	public:
		Client();
		Client(std::string pass, std::string nick, std::string user);
		~Client();

		void	setPass(std::string _pass);
		void	setNick(std::string _nick);
		void	setUser(std::string _user);
		std::string	getPass(void) const ;
		std::string	getNick(void) const ;
		std::string	getUser(void) const ;

};

#endif /* _CLIENT_HPP__ */
