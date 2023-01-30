
#ifndef _SERVER_HPP__
# define _SERVER_HPP__

# include "./ircserv.hpp"


# define MAX_CLIENT 17
# define TIMEOUT (10 * 60 * 1000)
# define DEFAULT_BUFLEN 512

class	Client;
class	Channel;
class server
{
	private:
		int			port;
		int			sock_fd;
		int			rc;
		int			timeout;
		std::string	password;
		bool		End_server;
		bool		st_conx;
		int			n_fds;
		int			i;
		std::string nick;
	public:
		socklen_t			addrLen;
		struct sockaddr_in	address;
		struct pollfd		fds[MAX_CLIENT];
		std::map<int, Client> myGuest;
		std::map<int, Client> myClient;
		std::map<std::string, Channel> channels;
		server();
		server(int _port, std::string _pswd);
		~server();
		//OCF

		int			getPort() const;
		std::string	getPassword() const;
		std::string		getNick() const ;
		void		setPort(int port);
		void		setNick(std::string _nick);
		void		setPassword(std::string password);

		void		start();
		void		CreateSocket();
		void		bindThesocket();
		bool		WaitClient();
		int			acceptSocket(int n_fds);
		bool		recvMessage(int i);
		//COMMANDS TO PARS START
		void		Parse_cmd(std::string input, int i);
		void 		Check_pass(std::string pass, std::string password, int i);
		void		Check_nick(std::string nick, int i);
		void		Check_user(std::string user, int i);
		void		Check_quit(int i);
		void 		Check_admin(int i);
		void 		Check_time(int i);
		void 		Check_who(std::string input, int i);
		void 		Check_privmsg(std::string input, int i);
		void 		Check_notice(std::string input, int i);
		void 		Check_dcc(std::string input, int i);
		void 		dcc_accept(std::string input, int i);
		void 		dcc_reject(std::string input, int i);
		void 		dcc_send(std::string input, int i);
		//channel start
		void		Check_join(std::string join, int fd);
		void		createChannel(std::string name, int fd);
		void		joinToChannel(std::string name, int fd);
		void 		check_users(std::string input, int i);
		void		check_exit_chan(std::string input, int fd);
		void		check_quit_chan(std::string input, int fd);
		void		add_op_chan(std::string input, int fd);
		void		kick_chan(std::string input, int fd);
		int			findMember(std::string namech, std::string nick);

		//channel end

		//COMMANDS TO PARS END
		std::string	_welcomemsg(int fd);
		void 		single_prvmsg(int source_fd, int destination_fd, std::string source, std::string message);

		class ErrorPortException : public std::exception
		{
			const char *what() const throw();
		};
};

char	**ft_split(char const *s, char c);
std::string printTime(void);

#endif /* _SERVER_HPP__ */