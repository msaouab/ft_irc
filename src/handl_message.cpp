
# include "../Includes/ircserv.hpp"

std::string	server::_welcomemsg(int fd)
{
	 std::string welcome ;
    welcome.append("001 " + myClient[fd].getNick() + " :Welcome\n");
    welcome.append("002 " + myClient[fd].getNick() + " :Greetings, fellow chatter! Welcome to our lovely IRC community.\n");
    welcome.append("003 " + myClient[fd].getNick() + " :If you need any help, simply type Bot :help for a list of available commands.\n");
    welcome.append("004 " + myClient[fd].getNick() + " :We're glad to have you here, let's make some memories!\n");
    return (welcome);
	return (welcome);
}

void	sendMsg(int fd, std::string msg)
{
	size_t i = 0;
	while (i != msg.length())
		i += send(fd, msg.c_str(), msg.length() - i, 0);
}