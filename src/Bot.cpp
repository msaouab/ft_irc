
#include "../Includes/Bot.hpp"
# include "../Includes/server.hpp"

Bot::Bot() {
}

Bot::~Bot() {
}

std::string	getPost(std::string input)
{
	std::string	post;
	std::string	c;
	char	**addr;
	if (input == "127.0.0.1") {
		post = "from localHost: 127.0.0.1";
		return post;
	}
	addr = ft_split(input.c_str(), '.');
	c = addr[1][1];
	post = "e";
	post.append(c);
	post.append("r");
	post.append(addr[2]);
	post.append("p");
	post.append(addr[3]);
	ft_free(addr);
	return post;
}

void	Bot::FIND(std::map<int, Client>myClient, char *input, int fd)
{
	std::string message;
	std::string	IPbuffer;

	std::map<int, Client>::iterator it;
	for (it = myClient.begin(); it != myClient.end(); it++) {
		if (it->second.getNick() == input) {
			message = "This person in <";
			message.append(getPost(myClient[it->first].getIP()));
			message.append(">\n");
			sendMsg(fd, ":BOT NOTICE " + myClient[fd].getNick() + " :" + message);
			return ;
		}
	}
	message = "Incorrect Nickname: ";
	message.append(input);
	sendMsg(fd, ":localhost 401 ERR_NOSUCHNICK :BOT: No such user\r\n");
}

void	Bot::TIME(std::map<int, Client>myClient, char *input, int fd)
{
	server s;
	std::string message;
	char buffer[64];
	struct timeval tv;
	long	time = 0;
	long	newTime;

	gettimeofday(&tv, NULL);
	newTime = tv.tv_sec;
	std::map<int, Client>::iterator it;
	for (it = myClient.begin(); it != myClient.end(); it++) {
		if (myClient[it->first].getNick() == input)
			time = myClient[it->first].getTime();
	}

	if (time == 0) {
		sendMsg(fd, ":localhost 401 ERR_NOSUCHNICK :BOT: No such user\r\n");
		return ;
	}

	newTime = newTime - time;
	struct tm *info = localtime(&newTime);
	strftime (buffer, sizeof(buffer), "%T", info);
	message = "Logtime of ";
	message.append(input);
	message.append(" <");
	message.append(buffer);
	message.append(">\n");
	sendMsg(fd, ":BOT NOTICE " + myClient[fd].getNick() + " :" + message);
}

void	Bot::HELP(std::map<int, Client>myClient, int fd)
{
	std::string message;

	message = "BOT :help - Prints this help message\n";
	if (!myClient[fd].getAuth()) {
		message.append("First thing you need to do is a enter the Commands to complet Connection Registration\n");
		message.append("PASS - password Server\n");
		message.append("NICK - <nickname> your Nickname\n");
		message.append("USER - your <username> <hostname> <servername> <realname>\n");
	}
	message.append("TIME - Show the current time\n");
	message.append("BOT :logtime - How much time connected in the server\n");
	message.append("BOT :find - find the Imac any one with you in the server\n");
	sendMsg(fd, ":BOT NOTICE " + myClient[fd].getNick() + " :" + message);
}

void	CreateBot(std::map<int, Client>myClient, std::string cmd, int fd)
{
	Bot			bot;
	char		**userArr;
	std::string	message;
	userArr = ft_split(cmd.c_str(), ' ');
	message.append("Please check '/BOT :help' for more information.\n");
	if (strcmp(userArr[1], ":help") == 0) {
		bot.HELP(myClient, fd);
		ft_free(userArr);
		return ;
	}
	if (lenArr(userArr) != 3) {
		ft_free(userArr);
		sendMsg(fd, message);
		return ;
	}
	if (strcmp(userArr[1], ":find") == 0)
		bot.FIND(myClient, userArr[2], fd);
	else if (strcmp(userArr[1], ":logtime") == 0)
		bot.TIME(myClient, userArr[2], fd);
	else
		sendMsg(fd, message);
	ft_free(userArr);
}