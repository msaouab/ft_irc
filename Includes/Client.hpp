#ifndef _CLIENT_HPP__
# define _CLIENT_HPP__

# include "./ircserv.hpp"

class Client
{
	private:
		std::string	nick;
		std::string	user;
		std::string	realname;
		std::string	ip;
		int			Clientfd;
		bool		Auth;
		bool		Log;
		long		time;
		bool		joinChan;
	public:
		Client();
		Client &operator=(const Client &rhs);
		bool operator==(const Client &rhs);
		~Client();

		void	setClientfd(int _clientfd);
		void	setNick(std::string _nick);
		void	setUser(std::string _user);
		void	setRealname(std::string real);
		void	setAuth(bool _Auth);
		void	setLog(bool _Auth);
		void	setIP(std::string _ip);
		void	setTime(long time);
		int			getClientfd(void) const ;
		std::string	getNick(void) const ;
		std::string getUser(void) const ;
		std::string getRealname(void) const ;
		std::string	getIP() const ;
		bool		getAuth() const ;
		bool		getLog() const ;
		long		getTime() const ;
		bool		getJoinChan() const;
		void	setJoinChan(bool _join);

};

std::ostream& operator<< (std::ostream& os, const Client& client);

#endif /* _CLIENT_HPP__ */

