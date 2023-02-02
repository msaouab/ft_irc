
# include "../Includes/ircserv.hpp"

std::string	server::_welcomemsg(int fd)
{
	(void)fd;
	std::string welcome ;
	welcome.append(":001 iq :Welcome\n");
	welcome.append("001 . :Greetings, fellow chatters! Welcome to our lively IRC community.\n");
	welcome.append("001 . :If you need any help, simply type Bot :help for a list of available commands.\n");
	welcome.append("001 . :We're glad to have you here, let's make some memories!\n");
	welcome.append("001 . :To connect with our server please enter: \n");
	welcome.append("002 . :Command: `PASS ` password server\n");
	welcome.append("003 . :Command: `NICK ` Your nickname in server please shoose one not unique\n");
	welcome.append("004 . :Command: `USER ` Your username in the server \n");
	return (welcome);
}

void	sendMsg(int fd, std::string msg)
{
	size_t i = 0;
	while (i != msg.length())
		i += send(fd, msg.c_str(), msg.length() - i, 0);
}