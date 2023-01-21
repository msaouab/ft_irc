
#include "../Includes/Bot.hpp"

Bot::Bot() {
}

Bot::~Bot() {
}

std::string	getPost(std::string input)
{
	std::string	post;
	std::string	c;
	char	**addr;

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
			sendMsg(fd, message, GREEN);
			return ;
		}
	}
	message = "Incorrect Nickname: ";
	message.append(input);
	sendMsg(fd, message, RED);
}

void	Bot::TIME(std::map<int, Client>myClient, char *input, int fd)
{
	(void)input;
	std::string message;
	char buffer[64];
	struct timeval tv;
	long	time;
	long	newTime;

	gettimeofday(&tv, NULL);
	newTime = tv.tv_sec;
	std::map<int, Client>::iterator it;
	for (it = myClient.begin(); it != myClient.end(); it++) {
		if (myClient[it->first].getNick() == input)
			time = myClient[it->first].getTime();
	}

	newTime = newTime - time;
	struct tm *info = localtime(&newTime);
	strftime (buffer, sizeof(buffer), "%T", info);
	message = "Logtime of ";
	message.append(input);
	message.append(" <");
	message.append(buffer);
	message.append(">\n");
	sendMsg(fd, message, GREEN);
	std::cout << buffer << std::endl;
}

void	CreateBot(std::map<int, Client>myClient, std::string cmd, int fd)
{
	Bot			bot;
	char		**userArr;
	std::string	message;
	userArr = ft_split(cmd.c_str(), ' ');
	message.append("Please check '/BOT :help' for more information.\n");
	// if (strcmp(userArr[1], ":help") == 0) {
	// 	bot.HELP(myClient, userArr[2], fd);
	// 	ft_free(userArr);
	// 	return ;
	// }
	if (lenArr(userArr) != 3) {
		ft_free(userArr);
		sendMsg(fd, message, RED);
		return ;
	}
	if (strcmp(userArr[1], ":find") == 0)
		bot.FIND(myClient, userArr[2], fd);
	else if (strcmp(userArr[1], ":logtime") == 0)
		bot.TIME(myClient, userArr[2], fd);
	else
		sendMsg(fd, message, RED);
	ft_free(userArr);
}

// void	Bot::CreateBot(int port)
// {
// 	int	client_fd;
// 	sockBot = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sockBot < 0) {
// 		std::cout << "Bot Socket() failed: " << strerror(errno) << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// 	addrBot.sin_family = AF_INET;
// 	addrBot.sin_port = htons(port);
// 	client_fd = connect(sockBot, (struct sockaddr *)&addrBot, addrLenBot);
// 	if (client_fd < 0)
// 		std::cout << "Connection failed" << std::endl;
// 	std::cout << "Bot Connection" << std::endl;
// 	// sendMsg()
// }
