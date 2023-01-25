
#include "../Includes/server.hpp"
#include "../Includes/Bot.hpp"

server::server() 
{

}

server::server(int _port, std::string _pswd) {
	this->port = _port;
	this->password = _pswd;
	this->n_fds = 1;
}

server::~server() {
}

int server::getPort() const
{
    return (this->port);
}

std::string	server::getPassword() const
{
    return (this->password);
}

void	server::setPort(int port)
{
    this->port = port;
}

void	server::setPassword(std::string password)
{
    this->password = password;
}
void	server::setNick(std::string _nick) {
	this->nick = _nick;
}

std::string		server::getNick() const {
	return (this->nick);
}

void	server::CreateSocket()
{
	int					opt;

	opt = 1;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		std::cout << "Socket() failed: " << strerror(errno) << '\n';
		exit(EXIT_FAILURE);
	}
	rc = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (rc < 0) {
		std::cout << "setsockopt() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	rc = fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	if (rc < 0) {
		std::cout << "fcntl() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
}

void	server::bindThesocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	rc = bind(sock_fd, (struct sockaddr *)&address, sizeof(address));
	if (rc < 0) {
		std::cout << "bind() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	rc = listen(sock_fd, 3);
	if (rc < 0) {
		std::cout << "listen() failed: " << strerror(errno) << '\n';
		close(sock_fd);
		exit(EXIT_FAILURE);
	}
	addrLen = sizeof(address);
	memset(fds, 0, sizeof(fds));
	std::cout << "Looking For Connections" << std::endl;
	fds[0].fd = sock_fd;
	fds[0].events = POLLIN;
}

bool	server::WaitClient()
{
	rc = poll(fds, n_fds, TIMEOUT);
	if (rc < 0) {
		std::cout << "poll() failed: " << strerror(errno) << '\n';
		return (false) ;
	}
	if (rc == 0) {
		std::cout << GRAY << "poll() timeout. End Program.\n" << ED << std::endl;
		return (false) ;
	}
	return (true);
}

int	server::acceptSocket(int n_fds)
{
	int	new_sd;
	std::string	ip_addr;

	new_sd = -1;
	while (new_sd == -1) {
		new_sd = accept(sock_fd, (struct sockaddr *)&address, &addrLen);
		if (new_sd < 0) {
			if (errno != EWOULDBLOCK) {
				std::cout << "accept() failed: " << strerror(errno) << '\n'<< std::endl;
				End_server = true;
			}
			break ;
		}
		std::string welcome = _welcomemsg(new_sd);
		std::cout << "New incomming connection ==> " << new_sd << std::endl;
		ip_addr = inet_ntoa(address.sin_addr);
		myGuest[new_sd].setIP(ip_addr);
		if (send(new_sd, welcome.c_str(), welcome.length(), 0) <= 0)
			std::cout << strerror(errno);
		fds[n_fds].fd = new_sd;
		fds[n_fds].events = POLLIN;
		n_fds++;
	}
	return (n_fds);
}

void	server::Check_pass(std::string pass, std::string password, int i)
{
	std::string message;
	pass = pass.substr(5, pass.length());
	message = "Incorrect Password\n> ";
	if(pass != password) 
	{
		myGuest[fds[i].fd].setAuth(false);
		sendMsg(fds[i].fd, message, RED);
		return ;
    }
		std::cout << "Fd in pass " << fds[i].fd << std::endl;
		myGuest.insert(std::pair<int, Client>(fds[i].fd, Client()));
		myGuest[fds[i].fd].setAuth(true);
}

void	server::Check_nick(std::string nick, int i)
{
	std::string	message;
	message = "You need to login so you can start chatting OR you can send HELP to see how :)\n>";
	std::string hash = "Please remove #/$ from your name\n>";
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, message, RED);
		return ;
	}
	if(std::count(nick.begin(), nick.end(), '#') || std::count(nick.begin(), nick.end(), '&'))
	{
		sendMsg(fds[i].fd, hash, RED);
		return ;
	}
	message = "this nickname already exist\n> ";
	nick = nick.substr(5, nick.length());
	std::map<int, Client>::iterator it;
	for (it = myGuest.begin(); it != myGuest.end(); it++) {
		if (it->second.getNick() == nick) {
			sendMsg(fds[i].fd, message, RED);
			return ;
		}
	}
	myGuest[fds[i].fd].setNick(nick);
	myGuest[fds[i].fd].setAuth(true);
	if (myGuest[fds[i].fd].getLog())
		myClient[fds[i].fd] = myGuest[fds[i].fd];

}

void	server::Check_user(std::string user, int i)
{
	char	**userArr;
	std::string	message;
	message = "You need to login so you can start chatting OR you can send HELP to see how :)\n>";
	std::cout << myGuest[fds[i].fd].getAuth() << std::endl;
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, message, RED);
		return ;
	}
	message = "Please enter your NICK before USER :)\n";
	if (myGuest[fds[i].fd].getNick() == "") {
		sendMsg(fds[i].fd, message, RED);
		return ;
	}
	user = user.substr(5, user.length());
	userArr = ft_split(user.c_str(), ' ');
	if (lenArr(userArr) < 4) {
		ft_free(userArr);
		message = "Command: USER.\nParameters: <username> <hostname> <servername> <realname>.\n>";
		sendMsg(fds[i].fd, message, RED);
		return ;
	}
	myGuest[fds[i].fd].setUser(userArr[0]);
	myGuest[fds[i].fd].setRealname(userArr[3]);
	if (myGuest[fds[i].fd].getAuth())
	{
		myClient[fds[i].fd] = myGuest[fds[i].fd];
		myGuest[fds[i].fd].setLog(true);
	}
	ft_free(userArr);
}

void	server::Check_quit(int i)
{
	std::string	message;
	message = "You are leaving the server.\nsee you later :)\n";
	sendMsg(fds[i].fd, message, GREEN);
	myGuest.erase(fds[i].fd);
	myClient.erase(fds[i].fd);
	std::map<int, Client>::iterator it;
	std::map<std::string, Channel>::iterator itChan;
	for(itChan = channels.begin(); itChan != channels.end(); itChan++)
	{
		itChan->second.usersAcitve.erase(fds[i].fd);
		itChan->second.usersChann.erase(fds[i].fd);
		itChan->second.modes.erase(fds[i].fd);
	}
	for (it = myGuest.begin(); it != myGuest.end(); it++) {
		std::cout << it->second.getClientfd() << " ==> " << it->second.getNick() << std::endl;
	}
	close(fds[i].fd);
}

void 	server::Check_admin(int i)
{
	std::string auterror;
	auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror, RED);
		return ;
	}
	std::string message;
	// std::map<int, Client>::iterator it = myClient.begin();
	// message = RED;
	message.append(":localhost 257 . :Your IRC server administrator's nickname is \r\n");
	// message.append(it->second.getNick());
	// message.append("\n> ");
	// message.append(ED);
	send(fds[i].fd, message.c_str(), message.length(), 0);
	return ;
	
}


std::string printTime(void)
{
	std::string _time;
	struct timeval tv;
	time_t	time;
	struct tm *info;
	gettimeofday(&tv, NULL);
	time = tv.tv_sec;
	info = localtime(&time);
	_time.append(asctime(info));
	_time.append("> ");
	return(_time);
}


void server::Check_time(int i)
{
	std::string p = "Today is";
	p.append(printTime());
	sendMsg(fds[i].fd, p, RED);
}

void 	server::Check_who(std::string input, int i) // add who for operators
{
	std::string auterror;
	std::string notFound;
	auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	notFound = ":localhost 352 " + this->getNick() + " :USER not found\r\n> ";
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror, RED);
		sendMsg(fds[i].fd, ":localhost 315 " + this->getNick() + " :END of /WHO list\r\n", RED);
		return ;
	}
	std::string message;
	std::map<int, Client>::iterator it;
	input = input.substr(4, input.length() - 4);
	for (it = myClient.begin(); it != myClient.end(); it++) {
		if (it->second.getNick() == input)
		{
			message = YELLOW;
			message.append("WHO request: Nickname ");
			message.append(it->second.getNick());
			message.append(" Username ");
			message.append(it->second.getUser());
			message.append(" And the real name is ");
			message.append(it->second.getRealname());
			send(fds[i].fd, message.c_str(), message.length(), 0);
			message = "\n> ";
			message.append(ED);
			send(fds[i].fd, message.c_str(), message.length(), 0);
			return ;
		}
	}
	sendMsg(fds[i].fd, notFound, RED);
	return ;
}

void server::single_prvmsg(int source_fd, int destination_fd, std::string source, std::string message)
{
	std::string prefix = " Message from ";
	prefix.append(source);
	prefix.append(" ");
	sendMsg(destination_fd, printTime(), GRAY);
	sendMsg(destination_fd, prefix, RED);
	sendMsg(destination_fd, message, ED);
	sendMsg(destination_fd, "\n> ", RED);
	sendMsg(source_fd, "Message sent !\n> ", RED);
}


void 	server::Check_privmsg(std::string input, int i) //TODO: user PRVIMSG on channels
{
	std::string	message;
	char **data;
	std::string auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	if (!myClient[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror, RED);
		return ;
	}
	message = "PRIVMSG: Syntax Error\n> ";
	input = input.substr(8, input.length());
	data = ft_split(input.c_str(), ' ');
	if (lenArr(data) < 2) 
	{
		ft_free(data);
		sendMsg(fds[i].fd, message, RED);
		return ;
	}
	std::map<int, Client>::iterator it;
	std::string destination = data[0];
	std::string msg;
	int n = 0;
	while(data[++n])
	{
		msg.append(data[n]);
		msg.append(" ");
	}
	ft_free(data);
	if(msg[0] == ':')
		msg = msg.substr(1, msg.length() - 1);
	for (it = myClient.begin(); it != myClient.end(); it++){
		if (it->second.getNick() == destination)
		{
			single_prvmsg(fds[i].fd, it->first, myClient[fds[i].fd].getNick(), msg);
			return ;
		}
			
	}
	std::map<std::string, Channel>::iterator itChann;
	std::string chan_msg = GREEN + destination + " " + ED + msg;
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->second.getName() == destination)
		{
			for (it = itChann->second.usersChann.begin() ; it != itChann->second.usersChann.end(); it++)
				single_prvmsg(fds[i].fd, it->first, myClient[fds[i].fd].getNick(), chan_msg);
			return ;
		}
	}
	sendMsg(fds[i].fd, "Destination not found!! \n> ", RED);
}

void	server::Check_notice(std::string input, int i)
{
  	std::string	message;
	char **data;
	std::string auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	if (!myClient[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror, RED);
		return ;
	}
	message = "NOTICE: Syntax Error\n> ";
	input = input.substr(7, input.length());
	data = ft_split(input.c_str(), ' ');
	if (lenArr(data) < 2 || data[1][0] != ':') 
	{
		ft_free(data);
		sendMsg(fds[i].fd, message, RED);
		return ;
	}
	std::map<int, Client>::iterator it;
	std::string destination = data[0];
	std::string msg;
	int n = 0;
	while(data[++n])
	{
		msg.append(data[n]);
		msg.append(" ");
	}
	ft_free(data);
	msg = msg.substr(1, msg.length() - 1);
	// std::map<std::string, Channel> channels;
	for (it = myClient.begin(); it != myClient.end(); it++)
	{
		if (it->second.getNick() == destination)
		{
			std::string prefix = " Notice from ";
			prefix.append(myClient[fds[i].fd].getNick());
			prefix.append(":\t");
			sendMsg(it->first, printTime(), GRAY);
			sendMsg(it->first, prefix, RED);
			sendMsg(it->first, msg, ED);
			sendMsg(it->first, "\n> ", RED);
			sendMsg(fds[i].fd, "Notice sent !\n> ", RED);
		}
			
	}
	sendMsg(fds[i].fd, "Destination not found!! \n> ", RED);

}


// void 	server::Check_dcc(std::string input, int i)
// {
// 	std::string auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
// 	if (!myClient[fds[i].fd].getAuth()) {
// 		sendMsg(fds[i].fd, auterror, RED);
// 		return ;
// 	}
// 	std::string	message = "DCC: NOT SUPPORTED!!\n> ";
// 	input = input.substr(4, input.length() - 4);
// 	if (!input.compare(0, 4, "SEND") && input.length() > 4)
// 		dcc_send(input, i);
// 	else if (!input.compare(0, 6, "ACCEPT") && input.length() > 6)
// 		dcc_accept(input, i);
// 	else if (!input.compare(0, 6, "REJECT") && input.length() > 6)
// 		dcc_reject(input, i);
// 	else
// 		sendMsg(fds[i].fd, message, RED);

// }

void server::joinToChannel(std::string name, int fd)
{
	std::cout << "fd is : " << fd << std::endl;
	std::string message = "hola in channel name's ";
	message.append(name);
	message.append("\n> ");
	sendMsg(fds[fd].fd, message, GREEN);
	int i = 0;
	std::map<int, Client>::iterator it;
	std::map<std::string, Channel>::iterator itChann;
	Client cl1;
	for (it = myClient.begin(); it != myClient.end(); it++)
	{
		if (it->first == fds[fd].fd)
		{
			cl1 = it->second;
			std::cout << " client is : " << cl1.getNick()<< std::endl;
			break;
		}
	}
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->second.getName() == name)
			break;
	}
	for (it = itChann->second.usersChann.begin() ; it != itChann->second.usersChann.end(); it++)
	{
		if (fd == it->first)
		{
			i = 1;
			break;
		}
	}
	std::cout <<"jkldsjkldsjfkldsj" << std::endl;
	cl1.setJoinChan(true);
	// 	it->second.setJoinChan(true);
	if (i == 0)
	{
		itChann->second.modes.insert(std::pair<int, std::string>(fds[fd].fd, "-a+r+w-b-o+i"));
		itChann->second.usersChann.insert(std::pair<int, Client>(fds[fd].fd, cl1));
	}
	it = itChann->second.usersChann.find(fds[fd].fd);
	if (it != itChann->second.usersChann.end())
	{
		it->second.setJoinChan(true);
	}
	// cl1.setJoinChan(true);
	itChann->second.usersAcitve.insert(std::pair<int, Client>(fds[fd].fd, it->second));
	// std::cout << "hola in channel name's: " << name << "and fd is : " << fd <<std::endl;
	message = "hola in channel name's ";
	message.append(name);
	message.append(" and fd is: ");
	message.append("\n> ");
	sendMsg(fds[fd].fd, message , GREEN);
}

void server::createChannel(std::string name, int chec, int fd)
{
	// std::map<int, Channel>::iterator it;
	if (chec == 0)
	{
			Channel c1 = Channel(name, "public", "nopass");
			std::map<int, Client>::iterator it;
			c1.modes.insert(std::pair<int, std::string>(fds[fd].fd, "all"));
			channels.insert(std::pair<std::string, Channel>(name, c1));
			joinToChannel(name, fd);
	}
	else
	{
		std::string message;
		char **chan = ft_split(name.c_str(), ' ');
		if (!chan[1])
		{
			ft_free(chan);
			message = "Wrong password\n> ";
			sendMsg(fds[i].fd, message, RED);
			/// *************************************************** USE ft_free **********************************************************
			return ;
		}
		name = chan[0];
		std::string pas = chan[1];
		Channel c1 = Channel(name, "private", pas);
		std::string type = "private";
		pas = chan[1];
		ft_free(chan);
		c1.setType(type);
		std::string modess = "all";
		c1.modes.insert(std::pair<int, std::string>(fds[fd].fd, modess));
		channels.insert(std::pair<std::string, Channel>(name, c1));
		joinToChannel(name, fd);
		/// *************************************************** USE ft_free **********************************************************
	}

}

void server::Check_join(std::string join, int fd)
{
	int checker = 0;
	char **chan = ft_split(join.c_str(), ' ');
	std::string name = chan[1], message;
	std::map<std::string , Channel>::iterator it;
	if (chan[1][0] != '#' && chan[1][0] != '&' && name.length() > 200)
	{
		/// *************************************************** USE ft_free **********************************************************
		ft_free(chan);
		sendMsg(fds[i].fd, "name of channel invalid", RED);
		return ;
	}
	if (chan[1] && chan[1][0] == '&')
	{
		if (!chan[2])
		{
			// std::cout << "password test" <<std::endl;
			message = "Please enter password\n";
			sendMsg(fds[fd].fd, message, RED);
			ft_free(chan);
			/// *************************************************** USE ft_free **********************************************************
			
			return ;
		}
		std::string psd = chan[2];
		std::cout << psd << std::endl;
		std::cout << "jkldsjfklds" << psd << std::endl;
		for (it = channels.begin(); it != channels.end(); it++)
		{
			std::cout << it->second.getName() << "fjdsklfjkldsfjdklsfjdkls"<< name << psd <<std::endl;
			if (it->first == name)
			{
				std::cout << "youssef irc " << it->second.getPassword() << std::endl;
				if (it->second.getPassword() == psd)
				{
					std::cout << "hollllllllllla" << std::endl;
					free(chan);
					joinToChannel(name, fd);
					return ;
				}
				else
				{
					ft_free(chan);
					/// *************************************************** USE ft_free **********************************************************
					message = "Password is wrong\n> ";
					sendMsg(fds[fd].fd, message, RED);
					return ;
				}
			}
			// else
			// {
			// }
		}
		checker = 1;
		name.append(" ");
		name.append(psd);
	}
	else
	{
		
		for (it = channels.begin(); it != channels.end(); it++)
		{
			if (it->first == name)
			{
					ft_free(chan);
					joinToChannel(name, fd);
					/// *************************************************** USE ft_free **********************************************************
					return ;
			}
		}
	}
	createChannel(name, checker, fd);
}

void server::check_users(std::string input, int fd)
{
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan || !chan[1])
		return ;
	std::string name = chan[1];
	ft_free(chan);
	std::map<int, Client>::iterator it;
	std::map<int, Client>::iterator it1;
	std::map<std::string, Channel>::iterator itChann;
	for (itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (name == itChann->first)
			break;
	}
	if (itChann == channels.end())
	{
		std::string message = "Channel name's " + name + "not found \n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	it1 = itChann->second.usersChann.find(fds[fd].fd);
	if (it1 == itChann->second.usersChann.end())
	{
		std::string message = "You are not member of channel " + name + "\n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	it1 = itChann->second.usersAcitve.find(fds[fd].fd);
	if (it1 == itChann->second.usersAcitve.end())
	{
		std::string message = "You are not member of channel " + name + "\n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	for (it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
	{
		std::string message = std::to_string(it->first) + " " + it->second.getNick();
		if (it->second.getJoinChan())
			message += " online\n";
		else
			message += " offline\n";
		sendMsg(fds[fd].fd, message, GREEN);
	}
}

void server::check_exit_chan(std::string input, int fd)
{
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan && !chan[1])
		return ;
	std::string name = chan[1];
	std::string message;
	ft_free(chan);
	std::map<int, Client>::iterator it;
	std::map<int, Client>::iterator it1;
	std::map<std::string, Channel>::iterator itChann;
	
	for (itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (name == itChann->first)
			break;
	}
	if (itChann == channels.end())
	{
		std::string message = "Channel name's " + name + "not found \n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	it1 = itChann->second.usersChann.find(fds[fd].fd);
	if (it1 == itChann->second.usersChann.end())
	{
		std::string message = "You are not member in channel " + name + "\n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	for(it = itChann->second.usersAcitve.begin(); it != itChann->second.usersAcitve.end(); it++)
	{
		if (it->first == fds[fd].fd)
			break;
	}
	if (it == itChann->second.usersAcitve.end())
	{
		std::string message = "You are not online in channel " + name + "\n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	for(it1 = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it1++)
	{
		if (it1->first == fds[fd].fd)
			break;
	}
	it1->second.setJoinChan(false);
	itChann->second.usersAcitve.erase(it->first);
	message = "See you soon\n";
	sendMsg(fds[fd].fd, message, RED);
}

void server::check_quit_chan(std::string input, int fd)
{
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan || !chan[1])
		return ;
	std::string name = chan[1];
	ft_free(chan);
	std::map<int, std::string>::iterator it;
	std::map<int, Client>::iterator it1;
	std::map<std::string, Channel>::iterator itChann;
	std::cout << "que mera bobos que mera bobos que mera bobos" << std::endl;
	for (itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (name == itChann->first)
			break;
	}
	if (itChann == channels.end())
	{
		std::string message = "Channel name's " + name + "not found \n";
		sendMsg(fds[fd].fd, message, RED);
		return ;
	}
	int i = 0;
	for (it = itChann->second.modes.begin(); it != itChann->second.modes.end(); it++)
	{
		if (it->second == "all")
			i++;
	}
	if (i == 1)
	{
		for (it1 = itChann->second.usersChann.begin(); it1 != itChann->second.usersChann.end(); it1++)
		{
			if (it1->first == fds[fd].fd)
				break ;
		}
		if (it1 == itChann->second.usersChann.end())
		{
			std::cout << "You are not exist in channel members" << std::endl;
			return ;
		}
		it1++;
		it = itChann->second.modes.find(it1->first);
		if (it != itChann->second.modes.end())
			it->second = "all";
	}
	itChann->second.modes.erase(fds[fd].fd);
	itChann->second.usersChann.erase(fds[fd].fd);
	// it1->second.setJoinChan(false);
	it1 = itChann->second.usersAcitve.find(fd);
	if (it1 != itChann->second.usersAcitve.end())
		itChann->second.usersAcitve.erase(fd);
	if (itChann->second.usersChann.size() == 0)
		channels.erase(itChann->first);
	std::string message = "You are now leave this channel\n";
	sendMsg(fds[fd].fd, message, RED);
}

void	server::Parse_cmd(std::string input, int i)
{
	std::string	message;
	std::cout << input << std::endl;

	message = "Input not supported\n> ";
	if (!input.compare(0, 4, "PASS") && input.length() > 4)
		Check_pass(input, password, i);
	else if (!(input.compare(0, 4, "NICK")) && input.length() > 4)
		Check_nick(input, i);
	else if (!(input.compare(0, 4, "USER")) && input.length() > 4)
		Check_user(input, i);
	else if (!(input.compare(0, 4, "QUIT")))
		Check_quit(i);
	else if (!(input.compare(0, 5, "ADMIN")))
		Check_admin(i);
	else if (!(input.compare(0, 4, "TIME")))
		Check_time(i);
	else if (!(input.compare(0, 3, "WHO")) && input.length() > 3)
		Check_who(input, i);
	else if (!(input.compare(0, 7, "PRIVMSG")) && input.length() > 7)
		Check_privmsg(input, i);
	else if (!(input.compare(0, 6, "NOTICE")) && input.length() > 6)
		Check_notice(input, i);
	// else if (!(input.compare(0, 3, "DCC")) && input.length() > 3)
	// 	Check_dcc(input, i);
	else if (!(input.compare(0, 4, "/BOT")) && input.length() > 4)
		CreateBot(myClient, input, fds[i].fd);
	else if (!(input.compare(0, 4, "JOIN")))
		Check_join(input, i);
	else if (!(input.compare(0, 10, "LISTUSERCH")))
		check_users(input, i);
	else if (!(input.compare(0, 6, "EXITCH")))
		check_exit_chan(input, i);
	else if (!(input.compare(0 , 6, "CHQUIT")))
		check_quit_chan(input, i);
	else
		sendMsg(fds[i].fd, message, RED);
}

bool	server::recvMessage(int i)
{
	char		buffer[DEFAULT_BUFLEN];
	std::string	input;
	
	rc = recv(fds[i].fd, buffer, DEFAULT_BUFLEN, 0);
	if (rc < 0) {
		if (errno != EWOULDBLOCK) {
			std::cout << "recv() failed: " << strerror(errno) << std::endl;
			st_conx = true;
		}
		return (false);
	}
	if (rc == 0) {
		std::cout << "Connection closed" << std::endl;
		myGuest.erase(fds[i].fd);
		st_conx = true;
		return (false);
	}
	buffer[rc] = '\0';
	if (rc != 1)
		input = strtok(buffer, "\r\n");
	Parse_cmd(input, i);
	return (true);
}

void	server::start()
{
	int					current_size;
	bool				compress_arr;
	int					i;

	n_fds = 1;
	current_size = 0;
	End_server = false;
	compress_arr = false;
	CreateSocket();
	bindThesocket();
	while (!End_server) {
		if (WaitClient() == false)
			break ;
		current_size = n_fds;
		for (i = 0; i < MAX_CLIENT; i++) {
			if (fds[i].revents == 0)
				continue ;
			if (fds[i].fd == sock_fd) {
				std::cout << "Listening Socker is readable\n" << std::endl;
				n_fds = acceptSocket(n_fds);
			}
			else {
				std::cout << "Discriptor " << fds[i].fd << " is readable\n" << std::endl;
				st_conx = false;
				std::string redr = "> ";
				sendMsg(fds[i].fd, redr, RED);
				recvMessage(i);
				// while (true) {
				// 	if (recvMessage(i) == false)
				// 		break ;
				// 	// sendMessage();
				// }
				while (true) {
					if (recvMessage(i) == false)
						break ;
				}
				if (st_conx) {
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_arr = true;
				}
			}
		}
	}
}

const char* server::ErrorPortException::what() const throw()
{
	return("\033[1;31m Error in Port number \033[0m");
}