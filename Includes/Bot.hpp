
#ifndef _BOT_HPP__
# define _BOT_HPP__

# include "./ircserv.hpp"
# include "./server.hpp"


class	Server;
class	Client;

class Bot
{
	private:
	public:
		Bot();
		~Bot();
		void	FIND(std::map<int, Client>myClient, char *input, int i);
		void	TIME(std::map<int, Client>myClient, char *input, int i);
		void	HELP(std::map<int, Client>myClient, int i);
};

void	CreateBot(std::map<int, Client>myClient, std::string cmd, int i);

#endif /* _BOT_HPP__ */
