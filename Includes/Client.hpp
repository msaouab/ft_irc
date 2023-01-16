#ifndef _CLIENT_HPP__
# define _CLIENT_HPP__

# include "./ircserv.hpp"

class Client
{
	private:
		std::string	nick;
		std::string	user;
		int			Clientfd;
	public:
		Client();
		Client &operator=(const Client &rhs);
		// bool operator<(const Client &rhs) const {
			// return it < rhs.it;
		// }
		Client( int fd );
		~Client();

		void	setClientfd(int _clientfd);
		void	setNick(std::string _nick);
		void	setUser(std::string	_user);
		int		getClientfd(void) const ;
		std::string	getNick(void) const ;
		std::string	getUser(void) const ;

};

std::ostream& operator<< (std::ostream& os, const Client& client);

#endif /* _CLIENT_HPP__ *