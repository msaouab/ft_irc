
# include "../Includes/ircserv.hpp"

std::string	server::_welcomemsg(void)
{
	struct timeval tv;
	time_t	time;
	struct tm *info;
	char buffer[64];
	gettimeofday(&tv, NULL);
	time = tv.tv_sec;
	info = localtime(&time);
	std::string welcome = GREEN;
	welcome.append("\n\n");
	welcome.append("\t██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
	welcome.append("\t██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
	welcome.append("\t██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗  \n");
	welcome.append("\t██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝  \n");
	welcome.append("\t╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
	welcome.append("\t ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n\n");
	welcome.append(ED);
	welcome.append(GRAY);
	welcome.append(asctime(info));
	strftime (buffer, sizeof buffer, "Today is %A, %B %d.\n", info);
	welcome.append(ED);
	welcome.append(BLUE);
	welcome.append("You need to login so you can start chatting OR you can send HELP to see how :) \n");
	welcome.append("To connect with our server please enter: \n");
	welcome.append("Command: `PASS ` password server\n");
	welcome.append("Command: `NICK ` Your nickname in server please shoose one not unique\n");
	welcome.append("Command: `USER ` Your username in the server \n");
	welcome.append(ED);
	return (welcome);
}

void	server::sendError(int fd, std::string msg, std::string color)
{
	std::string	message;
	message = color;
	message.append(msg);
	message.append(ED);
	send(fd, message.c_str(), message.length(), 0);
}