
#ifndef _SERVER_HPP__
# define _SERVER_HPP__

# include "./ircserv.hpp"
// # include "./Client.hpp"

# define MAX_CLIENT 17
# define TIMEOUT (10 * 60 * 1000)
# define DEFAULT_BUFLEN 512
class	Client;

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
		
		Client				*client;
		socklen_t			addrLen;
		struct sockaddr_in	address;
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
		bool		recvMessage(int i);
		void		sendMessage();

		class ErrorPortException : public std::exception
		{
			const char *what() const throw();
		};
};

void	Parse_Cmd(std::string input, std::string password, int client);
void 	Check_Pass(std::string input, std::string password);
void	Check_Nick(std::string input, int n);

#endif /* _SERVER_HPP__ */
