#ifndef _CLIENT_HPP__
# define _CLIENT_HPP__

# include "./ircserv.hpp"

class Client
{
	private:
		std::string	nick;
		char		**user;
		int			Clientfd;
		bool		Auth;
	public:
		Client();
		Client &operator=(const Client &rhs);
		~Client();

		void	setClientfd(int _clientfd);
		void	setNick(std::string _nick);
		void	setUser(char **_user);
		void	setAuth(bool _Auth);
		int			getClientfd(void) const ;
		std::string	getNick(void) const ;
		char	**getUser(void) const ;
		bool		getAuth() const ;

};

std::ostream& operator<< (std::ostream& os, const Client& client);

#endif /* _CLIENT_HPP__ */

