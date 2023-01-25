
# include "../Includes/ircserv.hpp"

std::string	server::_welcomemsg(int fd)
{
	struct timeval tv;
	time_t	time;
	struct tm *info;
	char buffer[64];
	gettimeofday(&tv, NULL);
	time = tv.tv_sec;
	std::cout << "time: ==> " << time << std::endl;
	myGuest[fd].setTime(time);
	info = localtime(&time);
	std::string welcome ;
	// welcome.append("\n\n");
	welcome.append(":001 iq :\t██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
	welcome.append("001 . :\t██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
	welcome.append("001 . :\t██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗  \n");
	welcome.append("001 . :\t██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝  \n");
	welcome.append("001 . :\t╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
	welcome.append("001 . :\t ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n\n");
	welcome.append(ED);
	welcome.append(GRAY);
	welcome.append(asctime(info));
	strftime (buffer, sizeof(buffer), "Today is %A, %B %d.\n", info);
	welcome.append(ED);
	welcome.append(BLUE);
	welcome.append("001 . :You need to login so you can start chatting OR you can send HELP to see how :) \n");
	welcome.append("001 . :To connect with our server please enter: \n");
	welcome.append("002 . :Command: `PASS ` password server\n");
	welcome.append("003 . :Command: `NICK ` Your nickname in server please shoose one not unique\n");
	welcome.append("004 . :Command: `USER ` Your username in the server \n");
	welcome.append(ED);
	welcome.append(RED);
	welcome.append("> ");
	welcome.append(ED);
	// welcome.append("\r\n");
	return (welcome);
}

void	sendMsg(int fd, std::string msg, std::string color)
{
	std::string	message;
	message = color;
	message.append(msg);
	message.append(ED);
	send(fd, message.c_str(), message.length(), 0);
}