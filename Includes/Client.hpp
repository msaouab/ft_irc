#ifndef _CLIENT_HPP__
# define _CLIENT_HPP__

# include "./ircserv.hpp"

class Client
{
	private:
		std::string	nick;
		std::string	user;
		std::string	realname;
		int			Clientfd;
		bool		Auth;
		bool		Log;
	public:
		Client();
		Client &operator=(const Client &rhs);
		~Client();

		void	setClientfd(int _clientfd);
		void	setNick(std::string _nick);
		void	setUser(std::string _user);
		void	setRealname(std::string real);
		void	setAuth(bool _Auth);
		void	setLog(bool _Auth);
		int			getClientfd(void) const ;
		std::string	getNick(void) const ;
		std::string getUser(void) const ;
		std::string getRealname(void) const ;
		bool		getAuth() const ;
		bool		getLog() const ;

};

std::ostream& operator<< (std::ostream& os, const Client& client);

#endif /* _CLIENT_HPP__ */

