# include "../Includes/server.hpp"
#include<sstream>
#include <iostream>

void	server::Check_pass(std::string pass, std::string password, int i)
{
	if(myGuest[fds[i].fd].getAuth())
	{
		sendMsg(fds[i].fd,":localhost 462 PASS :You may not reregister\n");
		return ;
	}
	if (pass.length() < 6)
	{
		sendMsg(fds[i].fd, ":localhost 461 PASS :Not enough parameters\n");
		return ;
	}
	std::string message;
	pass = pass.substr(5, pass.length());
	message = ":localhost 464 PASS :Password incorrect\n";
	if(pass != password) 
	{
		myGuest[fds[i].fd].setAuth(false);
		sendMsg(fds[i].fd, message);
		return ;
    }
	std::cout << "Fd in pass " << fds[i].fd << std::endl;
	myGuest.insert(std::pair<int, Client>(fds[i].fd, Client()));
	myGuest[fds[i].fd].setAuth(true);
}

void	server::Check_nick(std::string nick, int i)
{
	if (nick.length() < 6)
	{
		sendMsg(fds[i].fd, ":localhost 461 NICK :Not enough parameters\n");
		return ;
	}
	std::string	message;
	message = ":localhost 451 * NICK :You must finish connecting with pass first.\n";
	std::string hash = "Please remove #/$ from your name\n";
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, message);
		return ;
	}
	if(std::count(nick.begin(), nick.end(), '#') || std::count(nick.begin(), nick.end(), '&'))
	{
		sendMsg(fds[i].fd, hash);
		return ;
	}
	nick = nick.substr(5, nick.length());
	message = ":localhost 433 * " + nick + " :Nickname is already in use\n";
	std::map<int, Client>::iterator it;
	for (it = myGuest.begin(); it != myGuest.end(); it++) {
		if (it->second.getNick() == nick) {
			sendMsg(fds[i].fd, message);
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
	if (user.length() < 6)
	{
		sendMsg(fds[i].fd, ":localhost 461 USER :Not enough parameters\n");
		return ;
	}
	char	**userArr;
	std::string	message;
	message = ":localhost 451 * USER :You must finish connecting with pass and nickname first.\n";
	std::cout << myGuest[fds[i].fd].getAuth() << std::endl;
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, message);
		return ;
	}
	message = ":localhost 451 * USER :You must finish connecting with your nickname first.\n";
	if (myGuest[fds[i].fd].getNick() == "") {
		sendMsg(fds[i].fd, message);
		return ;
	}
	user = user.substr(5, user.length());
	userArr = ft_split(user.c_str(), ' ');
	if (lenArr(userArr) < 4) {
		ft_free(userArr);
		sendMsg(fds[i].fd, ":localhost 461 USER :Not enough parameters\n");
		return ;
	}
	myGuest[fds[i].fd].setUser(userArr[0]);
	myGuest[fds[i].fd].setRealname(userArr[3]);
	if (myGuest[fds[i].fd].getAuth())
	{
		myClient[fds[i].fd] = myGuest[fds[i].fd];
		myGuest[fds[i].fd].setLog(true);
	}
	std::string welcome = _welcomemsg(fds[i].fd);
	send(fds[i].fd,welcome.c_str(), welcome.length(), 0);

	ft_free(userArr);
}

void	server::Check_quit(int i)
{
	std::string	message;
	message = "You are leaving the server.see you later\n";
	sendMsg(fds[i].fd, message);
	myGuest.erase(fds[i].fd);
	myClient.erase(fds[i].fd);
	std::map<int, Client>::iterator it;
	std::map<std::string, Channel>::iterator itChan;
	for(itChan = channels.begin(); itChan != channels.end(); itChan++)
	{
		itChan->second.usersAcitve.erase(fds[i].fd);
		itChan->second.usersChann.erase(fds[i].fd);
		itChan->second.modes.erase(fds[i].fd);
		itChan->second.listBan.erase(fds[i].fd);
		itChan->second.invite_list.erase(fds[i].fd);
	}
	close(fds[i].fd);
}

void 	server::Check_admin(int i)
{
	std::string auterror;
	auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n";
	if (!myClient[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror);
		return ;
	}
	std::map<int, Client>::iterator it = myClient.begin();
	std::string message1,message2,message3,message4;
	message1.append(":localhost 256 " + myClient[fds[i].fd].getNick() + " :Administrative info about localhost\n");
	message2 = ":localhost 257 " + myClient[fds[i].fd].getNick() + " :" + it->second.getNick() + " is in Khouribga, Morocco\n";
	message1 = message1.append(message2);
	sendMsg(fds[i].fd,message1);
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
	_time.append("\n");
	return(_time);
}

void server::Check_time(int i)
{
	std::string p = ":localhost 391 " + this->getNick() + " localhost :Today is ";
	p.append(printTime());
	size_t il = 0;
	while (il != p.length())
		il +=send(fds[i].fd, p.c_str(), p.length() - il, 0);
}

void 	server::Check_who(std::string input, int i)
{
	std::string start,end;
	input = input.substr(4, input.length() - 4);
	std::map<int, Client>::iterator it;
	std::string auterror = ":localhost 451 * WHO :You must finish connecting with another nickname first.\n";
	end = ":localhost 315 " + myClient[fds[i].fd].getNick() + " " + input + " :END of /WHO list.\r\n";
	if (!myGuest[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror);
		return ;
	}
	for (it = myClient.begin(); it != myClient.end(); it++) {
		if (it->second.getNick() == input)
		{
			start = ":localhost 325 " + myClient[fds[i].fd].getNick() + " * ~" + it->second.getUser() + " " + it->second.getIP()\
			+ " localhost " + myClient[fds[i].fd].getNick() + " H:0 " + it->second.getRealname() + ".\n";
			sendMsg(fds[i].fd, start.append(end));
			return ;
		}
	}
	sendMsg(fds[i].fd, end);
	return ;
}

void server::single_prvmsg(int source_fd, int destination_fd, std::string source, std::string message)//DELETE LATER!!
{
	std::string prefix = " Message from ";
	prefix.append(source);
	prefix.append(" ");
	sendMsg(destination_fd, printTime());
	sendMsg(destination_fd, prefix);
	sendMsg(destination_fd, message);
	sendMsg(destination_fd, "\n");
	sendMsg(source_fd, "Message sent !\n");
}

void 	server::Check_privmsg(std::string input, int i)
{
	std::string auterror = ":localhost 451 * PRIVMSG :You must finish connecting with another nickname first.\n";
	if (!myClient[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror);
		return ;
	}
	std::string	message,to_send;
	char **data;
	message = "412 ERR_NOTEXTTOSEND :No text to send\n";
	if (input.length() < 9){
		sendMsg(fds[i].fd, ":localhost 461 PRIVMSG :Not enough parameters\n");
		return ;
	}
	input = input.substr(7, input.length());
	data = ft_split(input.c_str(), ' ');
	if (lenArr(data) < 2) 
	{
		ft_free(data);
		sendMsg(fds[i].fd, ":localhost 461 PRIVMSG :Not enough parameters\n");
		return ;
	}
	std::map<int, Client>::iterator it;
	std::string destination = data[0];
	char** multi_destination;
	bool multi = false;
	std::vector<std::string> desti_vect;
	if(std::count(destination.begin(), destination.end(),','))
	{
		multi_destination = ft_split(destination.c_str(),',');
		int len = 0;while(++len <= lenArr(multi_destination))
			desti_vect.push_back(multi_destination[len-1]);
	
		ft_free(multi_destination);
		multi = true;
	}
	else 
		desti_vect.push_back(destination);
	std::string msg;
	if (data[1][0] == ':')
	{
		int n = 0;
		while(data[++n])
		{
			msg.append(data[n]);
			msg.append(" ");
		}
		msg = msg.substr(1, msg.length() - 1);
	}
	else 
		msg = data[1];
	ft_free(data);
	for (it = myClient.begin(); it != myClient.end(); it++)
	{
		if (destination == myClient[fds[i].fd].getNick())
			return ;
		if (it->second.getNick() == destination && destination != myClient[fds[i].fd].getNick())
		{
			to_send = ":" + myClient[fds[i].fd].getNick() + " PRIVMSG " + destination + " :" + msg + "\n";
			sendMsg(it->first, to_send);
			return;
		}	
	}
	std::map<std::string, Channel>::iterator itChann;
	std::map<int ,Client>::iterator it1;
	std::map<int, std::string>::iterator it2;
	size_t vect_len = 0;
	size_t found = 0;
	if(!multi)
	{
		for(itChann = channels.begin(); itChann != channels.end(); itChann++)
		{
			if (itChann->second.getName() == destination && destination != this->getNick())
			{
				
				it1 = itChann->second.usersChann.find(fds[i].fd);
				if (it1 == itChann->second.usersChann.end())
				{
					sendMsg(fds[i].fd, ":localhost 442 "+ myClient[fds[i].fd].getNick() + " "+ destination + " :You're not on that channel\n");	
					return ;
				}
				found = itChann->second.getModeChan().find("+m");
				if (found != std::string::npos)
				{
					it2 = itChann->second.modes.find(fds[i].fd);
					if (it2 == itChann->second.modes.end())
					{
						sendMsg(fds[i].fd, ":localhost 404 "+ myClient[fds[i].fd].getNick() + " "+ destination + " :Cannot send to channel\n");
						return ;
					}
					found = it2->second.find("+v");
					if (found == std::string::npos)
					{
						sendMsg(fds[i].fd, ":localhost 404 "+ myClient[fds[i].fd].getNick() + " "+ destination + " :Cannot send to channel\n");
						return ;
					}
				}
				it2 = itChann->second.modes.find(fds[i].fd);
				found = it2->second.find("+b");
				if (found != std::string::npos)
				{
					sendMsg(fds[i].fd, ":localhost 404 "+ myClient[fds[i].fd].getNick() + " "+ destination + " :Cannot send to channel\n");
					return ;
				}
				for (it = itChann->second.usersChann.begin() ; it != itChann->second.usersChann.end(); it++)
				{	
					if (it->second.getNick() == myClient[fds[i].fd].getNick())
						i *= 1;
					else
					{
							to_send = ":" + myClient[fds[i].fd].getNick() + " PRIVMSG " + destination + " :" + msg + "\n";
							sendMsg(it->first, to_send);
					}
				}
				return ;
			}
		}
	}
	else
	{
		while(vect_len < desti_vect.size())
		{
			destination = desti_vect.at(vect_len);
			for (itChann = channels.begin(); itChann != channels.end(); itChann++)
			{
				if (itChann->second.getName() == destination && destination != this->getNick())
				{
					it1 = itChann->second.usersChann.find(fds[i].fd);
					if (it1 == itChann->second.usersChann.end())
					{
						sendMsg(fds[i].fd, ":localhost 403 "+ myClient[fds[i].fd].getNick() + " "+ destination + " :No such channel\n");
						break ;
					}
					for (it = itChann->second.usersChann.begin() ; it != itChann->second.usersChann.end(); it++)
					{	
						if (it->second.getNick() == myClient[fds[i].fd].getNick())
							i *= 1;
						else
						{
							to_send = ":" + myClient[fds[i].fd].getNick() + " PRIVMSG " + destination + " :" + msg + "\n";
							sendMsg(it->first, to_send);
						}
					}
					break ;
				}
			}
			vect_len++;
		}
		return ;
	}
	sendMsg(fds[i].fd, ":localhost 401 ERR_NOSUCHNICK :No such user/channel\r\n");
}

void	server::Check_notice(std::string input, int i)
{
  	std::string	message,to_send;
	char **data;
	std::string auterror = ":localhost 451 * NOTICE :You must finish connecting with another nickname first.\n";
	if (!myClient[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror);
		return ;
	}
	input = input.substr(6, input.length());
	data = ft_split(input.c_str(), ' ');
	if (lenArr(data) < 2) 
	{
		ft_free(data);
		sendMsg(fds[i].fd, ":localhost 461 " + myClient[fds[i].fd].getNick() + " NOTICE :Not enough parameters\n");
		return ;
	}
	std::map<int, Client>::iterator it;
	std::string destination = data[0];
	std::string msg;
	if (data[1][0] == ':')
	{
		int n = 0;
		while(data[++n])
		{
			msg.append(data[n]);
			msg.append(" ");
		}
		msg = msg.substr(1, msg.length() - 1);
	}
	else 
		msg = data[1];
	ft_free(data);
	for (it = myClient.begin(); it != myClient.end(); it++)
	{
		if (it->second.getNick() == destination && destination != myClient[fds[i].fd].getNick())
		{
			to_send = ":" + myClient[fds[i].fd].getNick() + " NOTICE " + destination + " :" + msg + "\n";
			sendMsg(it->first, to_send);
			return;
		}	
	}
	sendMsg(fds[i].fd, ":localhost 401 ERR_NOSUCHNICK :No such user\r\n");
}

void server::joinToChannel(std::string name, int fd)
{
	std::string message;
	int i = 0;
	std::map<int, Client>::iterator it;
	std::map<std::string, Channel>::iterator itChann;
	Client cl1;
	for (it = myClient.begin(); it != myClient.end(); it++)
	{
		if (it->first == fds[fd].fd)
		{
			cl1 = it->second;
			break;
		}
	}
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->second.getName() == name)
			break;
	}
	if (itChann == channels.end())
	{
		sendMsg(fds[fd].fd, ":localhost 403 "+ myClient[fds[fd].fd].getNick() + " "+ name + " :No such channel\n");
		return ;
	}
	for (it = itChann->second.usersChann.begin() ; it != itChann->second.usersChann.end(); it++)
	{
		if (fds[fd].fd == it->first)
		{
			i = 1;
			break;
		}
	}
	for(it = itChann->second.listBan.begin(); it != itChann->second.listBan.end(); it++)
	{
		if (it->first == fds[fd].fd)
		{
			message = "localhost 474 JOIN " + itChann->second.getName() + " :Cannot join channel\n";
			sendMsg(fds[fd].fd, message);
			return ;
		}
	}
	cl1.setJoinChan(true);
	if (i == 0)
	{
		itChann->second.usersChann.insert(std::pair<int, Client>(fds[fd].fd, cl1));
	}
	it = itChann->second.usersChann.find(fds[fd].fd);
	if (it != itChann->second.usersChann.end())
	{
		it->second.setJoinChan(true);
	}
	itChann->second.usersAcitve.insert(std::pair<int, Client>(fds[fd].fd, it->second));
	message = ":" + it->second.getNick() + " JOIN :" + itChann->second.getName() + "\n";
	sendMsg(fds[fd].fd, message);
	check_users("NAMES "+name, fd);
}

void server::createChannel(std::string name, int fd)
{
	Channel c1 = Channel(name, " ");
	std::map<int, Client>::iterator it;
	c1.modes.insert(std::pair<int, std::string>(fds[fd].fd, "+O+o+v"));
	c1.setTime(printTime());
	channels.insert(std::pair<std::string, Channel>(name, c1));
	joinToChannel(name, fd);
}

void server::Check_join(std::string join, int fd)
{
	std::string auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	if (!myGuest[fds[fd].fd].getAuth()) {
		sendMsg(fds[fd].fd, auterror);
		return ;
	}
	std::map<int, Client>::iterator it2;
	for (it2 = myClient.begin(); it2 != myClient.end(); it2++)
	{
		if (it2->first == fds[fd].fd)
		{
			break;
		}
	}
	if (it2 == myClient.end())
	{
		sendMsg(fds[fd].fd, ":localhost 451 * JOIN :You must finish connecting with another nickname first.\n");
		return;
	}
	std::string enough = ":localhost 461 " + myClient[fds[fd].fd].getNick() + " JOIN :Not enough parameters\n";
	std::string psd ;
	char **chan = ft_split(join.c_str(), ' ');
	if (lenArr(chan) == 1 || lenArr(chan) == 2 || lenArr(chan) >= 3)
	{
		if(lenArr(chan) == 1)
		{
			sendMsg(fds[fd].fd, enough);
			ft_free(chan);
			return ;
		}
		else if (lenArr(chan) == 2)
		{
			std::string namech = chan[1];
			if (namech.size() == 1 && namech.c_str()[0] == '#')
			{
				sendMsg(fds[fd].fd, enough);
				ft_free(chan);
				return ;
			}
		}
		if(lenArr(chan) >= 3)
		{
			psd = chan[2];
		}
	}
	std::string name = chan[1], message;
	std::string param = chan[1];
	if (lenArr(chan) < 2 || lenArr(chan) > 3 || (param.c_str()[0] != '#'))
	{
		ft_free(chan);
		sendMsg(fds[fd].fd, ":localhost 403 "+ myClient[fds[fd].fd].getNick() + " "+ name + " :No such channel\n");
		return ;
	}
	std::map<std::string , Channel>::iterator it;
	if (chan[1][0] != '#' && name.length() > 200)
	{
			ft_free(chan);
			message = ":localhost 403 "+ myClient[fds[fd].fd].getNick() + " "+ name + " :No such channel\n";
			sendMsg(fds[fd].fd, message);
			return ;
	}
	for (it = channels.begin(); it != channels.end(); it++)
	{
		if (it->first == name)
		{
			size_t found = it->second.getModeChan().find("+k");
			if (found != std::string::npos)
			{
				if (it->second.getPassword().size() == 0 || !chan[2])
				{
					sendMsg(fds[fd].fd, ":localhost 475 "+ myClient[fds[fd].fd].getNick() + " " + name + " :Cannot join channel (+k)\n");
					ft_free(chan);
					return;
				}
				else if (chan[2])
				{
					if (psd != it->second.getPassword())
					{
						sendMsg(fds[fd].fd, ":localhost 475 "+ myClient[fds[fd].fd].getNick() + " " + name + " :Cannot join channel (+k)\n");
						ft_free(chan);
						return;
					}
				}
			}
			found = it->second.getModeChan().find("+l");
			if (found != std::string::npos)
			{
				int s = it->second.usersChann.size();
				if(it->second.getSizeOfUsers() == s)
				{
					sendMsg(fds[fd].fd, ":localhost 475 "+ myClient[fds[fd].fd].getNick() + " " + name + " :Cannot join channel (+l)\n");
					ft_free(chan);
					return;
				}
			}
			found = it->second.getModeChan().find("+i");
			if (found != std::string::npos)
			{
				std::map<int,Client>::iterator cl;
				cl = it->second.invite_list.find(fds[fd].fd);
				if(cl == it->second.invite_list.end())
				{
					sendMsg(fds[fd].fd, ":localhost 475 "+ myClient[fds[fd].fd].getNick() + " " + name + " :Cannot join channel (+i)\n");
					ft_free(chan);
					return;
				}
			}
			it2 = it->second.usersChann.find(fds[fd].fd);
			if (it2 != it->second.usersChann.end())
			{
				ft_free(chan);
				return;
			}
			ft_free(chan);
			joinToChannel(name, fd);
			return ;
		}
	}
	ft_free(chan);
	createChannel(name, fd);
}

void server::check_users(std::string input, int fd)
{
	int i = 0;
	std::string message;
	std::string auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	if (!myGuest[fds[fd].fd].getAuth()) {
		sendMsg(fds[fd].fd, auterror);
		return ;
	}
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan || !chan[1])
	{
		ft_free(chan);
		return ;
	}
	std::string enough = ":localhost 461 " + myClient[fds[fd].fd].getNick() + " JOIN :Not enough parameters\n";
	if (lenArr(chan) == 1 || lenArr(chan) == 2 || lenArr(chan) >= 3)
	{
		if(lenArr(chan) == 1)
		{
			ft_free(chan);
			sendMsg(fds[fd].fd, enough);
			return ;
		}
		else if (lenArr(chan) == 2)
		{
			std::string namech = chan[1];
			if (namech.size() == 1 && namech.c_str()[0] == '#')
			{
				ft_free(chan);
				sendMsg(fds[fd].fd, enough);
				return ;
			}
		}
	}
	std::string name = chan[1];
	ft_free(chan);
	std::map<int, Client>::iterator it;
	std::map<int, Client>::iterator it1;
	std::map<int, std::string>::iterator itmode;
	std::map<std::string, Channel>::iterator itChann;
	std::vector<std::string> myvector;
	for (itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (name == itChann->first)
			break ;
	}
	if (itChann == channels.end())
	{
		std::string message = ":localhost 366 " + myClient[fds[fd].fd].getNick() + " " + name + " :End of /NAMES list.\n";
		sendMsg(fds[fd].fd, message);
		return ;
	}
	it1 = itChann->second.usersChann.find(fds[fd].fd);
	if (it1 == itChann->second.usersChann.end())
	{
		std::string message = ":localhost 353 " + myClient[fds[fd].fd].getNick() + " = " + name + " :\n";
		sendMsg(fds[fd].fd, message);
		sendMsg(fds[fd].fd, ":localhost 366 " + myClient[fds[fd].fd].getNick() + " " + name + " :End of /NAMES list.\n");
		return ;
	}
	for (it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
	{
		if (i == 0)
			message = ":localhost 353 " + myClient[fds[fd].fd].getNick() + " = " + itChann->second.getName() + " :" ;
		myvector.push_back(it->second.getNick());
		i++;
	}
	while(--i)
		message.append(myvector.at(i) + " ");
	sendMsg(fds[fd].fd, message.append("@" + myvector.front() + "\n"));
	sendMsg(fds[fd].fd, ":localhost 366 " + myClient[fds[fd].fd].getNick() + " " + itChann->second.getName()+ " :End of /NAMES list.\n");
}

void server::check_quit_chan(std::string input, int fd)
{
	std::string auterror = "You need to login so you can start chatting OR you can send HELP to see how :)\n> ";
	if (!myGuest[fds[fd].fd].getAuth()) {
		sendMsg(fds[fd].fd, auterror);
		return ;
	}
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan || !chan[1])
	{
		ft_free(chan);
		sendMsg(fds[fd].fd, ":localhost 461 " + myClient[fds[fd].fd].getNick() + " PART :Not enough parameters\n");
		return ;
	}
	std::string name = chan[1];
	ft_free(chan);
	std::map<int, std::string>::iterator it;
	std::map<int, Client>::iterator it1;
	std::map<std::string, Channel>::iterator itChann;
	for (itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (name == itChann->first)
			break;
	}
	if (itChann == channels.end())
	{
		sendMsg(fds[fd].fd, ":localhost 403 "+ myClient[fds[fd].fd].getNick() + " "+ name + " :No such channel\n");
		return ;
	}
	for (it1 = itChann->second.usersChann.begin(); it1 != itChann->second.usersChann.end(); it1++)
	{
		if (it1->first == fds[fd].fd)
			break ;
	}
	if (it1 == itChann->second.usersChann.end())
	{
		sendMsg(fds[fd].fd, ":localhost 442 "+ myClient[fds[fd].fd].getNick() + " "+ name + " :You're not on that channel\n");
		return ;
	}
	std::string message = ":" + myClient[fds[fd].fd].getNick() + " PART " + name +"\n";
	for(it1 = itChann->second.usersChann.begin(); it1 != itChann->second.usersChann.end(); it1++)
	{
		sendMsg(it1->first, message);
	}
	itChann->second.modes.erase(fds[fd].fd);
	itChann->second.usersChann.erase(fds[fd].fd);
	if (itChann->second.usersChann.size() == 0)
		channels.erase(itChann->first);
}

int server::findMember(std::string namech, std::string nick)
{
	std::map<int, Client>::iterator it;
	std::map<std::string, Channel>::iterator itChann;
	std::map<int, std::string>::iterator itmode;
	std::string message;
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->first == namech)
			break;
	}
	for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
	{
		if(it->second.getNick() == nick)
		{
			return it->first;
		}
	}
	return(0);
}

int checkmode(std::string m)
{
	if (m == "+O" || m == "+o" || m == "-o" || 
		m == "+b" || m == "-b" || m == "+v" || m == "-v"||
		m == "+l" || m == "+k" )
		return (1);
	else if (m == "+m" || m == "-m" ||
		m == "+i" || m == "-i" || m == "-l" || m == "-k")
		return (2);
	return (0);
}

void server::mode_Arr_Three(char **chan, int fd)
{
	
	std::string namechan = chan[1];
	std::string mode = chan[2];
	size_t found;
	std::map<std::string , Channel>::iterator itChann;
	std::map<int, std::string>::iterator it1;
	std::map<int, Client>::iterator it;

	if (checkmode(mode) == 2)
	{

		for(itChann = channels.begin(); itChann != channels.end(); itChann++)
		{
			if (itChann->first == namechan)
				break;
		}
		if (itChann == channels.end())
		{
			sendMsg(fds[fd].fd, ":localhost 403 " + myClient[fds[fd].fd].getNick()+ " " + namechan +  " :No such channel\n");
			ft_free(chan);
			return ;
		}
		it = itChann->second.usersChann.find(fds[fd].fd);
		if (it == itChann->second.usersChann.end())
		{
			sendMsg(fds[fd].fd, ":localhost 442 "+ myClient[fds[fd].fd].getNick() + " "+ namechan + " :You're not on that channel\n");
			ft_free(chan);
			return ;
		}
		if (checkmode(mode) == 0)
		{
			ft_free(chan);
			return ;
		}
		if (checkmode(mode) == 1)
		{
			sendMsg(fds[fd].fd, ":localhost 461 " + myClient[fds[fd].fd].getNick() + " MODE " + mode + " :Not enough parameters\n");
			ft_free(chan);
			return ;	
		}
		it1 = itChann->second.modes.find(fds[fd].fd);
		if (it1 == itChann->second.modes.end())
		{
			sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
			ft_free(chan);
			return ;
		}
		found = it1->second.find("+O");
		if (found == std::string::npos)
		{
			found = it1->second.find("+o");
			if (found == std::string::npos)
			{
				sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
				ft_free(chan);
				return ;
			}
		}
		if (mode.c_str()[0] == '+')
		{
			std::string str = itChann->second.getModeChan();
			found  = itChann->second.getModeChan().find(mode);
			if (found == std::string::npos)
				itChann->second.setModeChan(str.append(mode));
			for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
			{
				sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + "\n");
			}
			ft_free(chan);
			return ;
		}
		else if (mode.c_str()[0] == '-')
		{
			std::string str = mode;
			str.replace(0, 1, "+");
			found = itChann->second.getModeChan().find(str);
			if (found != std::string::npos)
			{
				std::string rem = itChann->second.getModeChan();
				rem.erase(found, 2);
				if (rem.size() > 0)
				{
					itChann->second.setModeChan(rem);
				}
			}
			for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
			{
				sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + "\n");
			}
			ft_free(chan);
			return ;
		}
	}
	else if (checkmode(mode) == 1)
	{
		std::string enough = ":localhost 461 " + myClient[fds[fd].fd].getNick() + " MODE :Not enough parameters\n";
		sendMsg(fds[fd].fd, enough);
		ft_free(chan);
		return ;
	}
	ft_free(chan);
}

void server::mode_Arr_Two(char **chan, int fd)
{
	std::map<std::string , Channel>::iterator itChann;
	std::string namechan = chan[1];
	std::map<int, Client>::iterator it;
	std::map<int, std::string>::iterator it1;

	int i = 0;
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->first == namechan)
			break;
	}
	if (itChann == channels.end())
	{
		sendMsg(fds[fd].fd, ":localhost 403 " + myClient[fds[fd].fd].getNick()+ " " + namechan +  " :No such channel\n");
		ft_free(chan);
		return ;
	}
	it1 = itChann->second.modes.find(fds[fd].fd);
	it = itChann->second.usersChann.find(fds[fd].fd);
	if (it == itChann->second.usersChann.end())
	{
		sendMsg(fds[fd].fd, ":localhost 442 "+ myClient[fds[fd].fd].getNick() + " "+ namechan + " :You're not on that channel\n");
		ft_free(chan);
		return ;
	}
	size_t found = itChann->second.getModeChan().find("+k");
	if (found != std::string::npos)
	{
		i = 1;
		sendMsg(fds[fd].fd, ":localhost 324 " + myClient[fds[fd].fd].getNick()+ " " + namechan + " " + itChann->second.getModeChan()+  " " + itChann->second.getPassword());
		
	}
	found = itChann->second.getModeChan().find("+l");
	if (found != std::string::npos)
	{
		std::string size;
		std::stringstream ss;
		int j = itChann->second.getSizeOfUsers();
		ss << j;    
		ss >> size;  
		if (i == 0)
		{
			i = 2;
			sendMsg(fds[fd].fd, ":localhost 324 " + myClient[fds[fd].fd].getNick()+ " " + namechan + " " + itChann->second.getModeChan()+  " " + size );
		}
		else
		{
			sendMsg(fds[fd].fd, " " + size);
		}
	}
	if (i == 0)
	{
		sendMsg(fds[fd].fd, ":localhost 324 " + myClient[fds[fd].fd].getNick()+ " " + namechan + " " + itChann->second.getModeChan()+"\n");
		sendMsg(fds[fd].fd, ":localhost 329 " + myClient[fds[fd].fd].getNick()+ " " + namechan + " " + itChann->second.getTime());
		ft_free(chan);
		return ;

	}
	sendMsg(fds[fd].fd, "\n:localhost 329 " + myClient[fds[fd].fd].getNick()+ " " + namechan + " " + itChann->second.getTime());
	ft_free(chan);
	return;
}

void server::mode_Arr_four(char **chan , int fd)
{
	std::string namechan = chan[1];
	std::string mode = chan[2];
	std::string size = chan[3];
	size_t found;
	std::map<std::string , Channel>::iterator itChann;
	std::map<int, std::string>::iterator it1;
	std::map<int, Client>::iterator it;

	std::string enough = ":localhost 461 " + myClient[fds[fd].fd].getNick() + " MODE :Not enough parameters\n";
	if (!chan[1])
	{
		sendMsg(fds[fd].fd, enough);
		ft_free(chan);
		return ;
	}
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->first == namechan)
			break;
	}
	if (itChann == channels.end())
	{
		sendMsg(fds[fd].fd, ":localhost 403 " + myClient[fds[fd].fd].getNick()+ " " + namechan +  " :No such channel\n");
		ft_free(chan);
		return ;
	}
	it = itChann->second.usersChann.find(fds[fd].fd);
	if (it == itChann->second.usersChann.end())
	{
		sendMsg(fds[fd].fd, ":localhost 442 "+ myClient[fds[fd].fd].getNick() + " "+ namechan + " :You're not on that channel\n");
		ft_free(chan);
		return ;
	}
	if (mode == "+l")
	{
		if (!chan[3])
		{
			ft_free(chan);
			return ;
		}
		else
		{
			it1 = itChann->second.modes.find(fds[fd].fd);
			found = it1->second.find("+O");
			if (found == std::string::npos)
			{
				found =it1->second.find("+o");
				if (found == std::string::npos)
				{
					sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
					ft_free(chan);
					return ;
				}
			}
			if (isNumber(size))
			{
				if (stoi(size) == 0)
				{
					ft_free(chan);
					return;
				}
				std::string str = itChann->second.getModeChan();
				found  = itChann->second.getModeChan().find(mode);
				if (found == std::string::npos)
					itChann->second.setModeChan(str.append(mode));
				itChann->second.setSize(stoi(size));
				for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
				{
					sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "\n");
				}
				ft_free(chan);
				return ;
			}
			else
			{
				ft_free(chan);
				return ;
			}
		}
	}
	else if (mode == "+k")
	{
		it1 = itChann->second.modes.find(fds[fd].fd);
		found = it1->second.find("+O");
		if (found == std::string::npos)
		{
			found =it1->second.find("+o");
			if (found == std::string::npos)
			{
				sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
				ft_free(chan);
				return ;
			}
		}
		found  = itChann->second.getModeChan().find(mode);
		if (found == std::string::npos)
		{
			std::string str = itChann->second.getModeChan();
			itChann->second.setModeChan(str.append(mode));
		}
		itChann->second.setPassword(size);
		for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
		{
			sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "\n");
		}
		ft_free(chan);
		return ;
	}
	else
	{
		if (checkmode(mode) == 1)
		{
			for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
			{
				if (it->second.getNick() == size)
					break;
			}
			if (it == itChann->second.usersChann.end())
			{
				sendMsg(fds[fd].fd, ":localhost 401 "+ myClient[fds[fd].fd].getNick()+ " " + size +" :No such nick/channel\n");
				sendMsg(fds[fd].fd, ":localhost 441 " + myClient[fds[fd].fd].getNick()+ " " + size + " " + namechan + " :They aren't on that channel\n");
				ft_free(chan);
				return ;
			}
			if (mode != "+b" && mode.c_str()[0] == '+')
			{
				if (fds[fd].fd == it->first)
					return;
				it1 = itChann->second.modes.find(fds[fd].fd);
				found = it1->second.find("+O");
				if (found == std::string::npos)
				{
					found =it1->second.find("+o");
					if (found == std::string::npos)
					{
						sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
						ft_free(chan);
						return ;
					}
				}
				it1 = itChann->second.modes.find(it->first);
				if (it1 == itChann->second.modes.end())
				{
					found = it1->second.find("+O");
					if (found == std::string::npos)
					{
						itChann->second.modes.insert(std::pair<int , std::string>(it->first, mode));
						for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
						{
							sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "\n");
						}
					}
					ft_free(chan);
					return ;
				}
				else
				{
					it1 = itChann->second.modes.find(fds[fd].fd);
					found = it1->second.find("+O");
					if (found == std::string::npos)
					{
						found =it1->second.find("+o");
						if (found == std::string::npos)
						{
							sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
							ft_free(chan);
							return ;
						}
					}
					it1 = itChann->second.modes.find(it->first);
					found = it1->second.find("+O");
					if (found == std::string::npos) 
					{
						found = it1->second.find(mode);
						if (found == std::string::npos)
						{
							std::string str = it1->second;
							str.append(mode);
							itChann->second.modes.erase(it->first);
							itChann->second.modes.insert(std::pair<int , std::string>(it->first, str));
						}
						for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
						{
							sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "\n");
						}
					}
					ft_free(chan);
					return ;
				}
			}
			else if (mode == "+b")
			{
				if (fds[fd].fd == it->first)
					return;
				it1 = itChann->second.modes.find(fds[fd].fd);
				found = it1->second.find("+O");
				if (found == std::string::npos)
				{
					found =it1->second.find("+o");
					if (found == std::string::npos)
					{
						sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
						ft_free(chan);
						return ;
					}
				}
				it1 = itChann->second.modes.find(it->first);
				found = it1->second.find("+O");
				if (found == std::string::npos)
				{
					if (it1 == itChann->second.modes.end())
					{
						itChann->second.modes.insert(std::pair<int , std::string>(it->first, mode));
					}
					else
					{
						found = it1->second.find(mode);
						if (found == std::string::npos)
						{

							std::string str = it1->second;
							str.append(mode);
							itChann->second.modes.erase(it->first);
							itChann->second.modes.insert(std::pair<int , std::string>(it->first, str));
							itChann->second.listBan.insert(std::pair<int, Client>(it->first, it->second));
						}
						else
						{
							ft_free(chan);
							return;
						}
					}	
					for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
					{
						sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "!*@*\n");
					}
				}
				ft_free(chan);
				return ;
			}
			else if (mode != "-b" && mode.c_str()[0] == '-')
			{
				it1 = itChann->second.modes.find(fds[fd].fd);
				found = it1->second.find("+O");
				if (found == std::string::npos)
				{
					found =it1->second.find("+o");
					if (found == std::string::npos)
					{
						sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + namechan + " :You're not channel operator\n");
						ft_free(chan);
						return ;
					}
				}
				for(it = itChann->second.usersChann.begin();it != itChann->second.usersChann.end(); it++)
				{
					if (it->second.getNick() == size)
						break;
				}
				if (it == itChann->second.usersChann.end())
				{
					sendMsg(fds[fd].fd, ":localhost 401 "+ myClient[fds[fd].fd].getNick()+ " " + size +" :No such nick/channel\n");
					sendMsg(fds[fd].fd, ":localhost 441 " + myClient[fds[fd].fd].getNick()+ " " + size + " " + namechan + " :They aren't on that channel\n");
					ft_free(chan);
					return ;
				}
				if (fds[fd].fd == it->first)
					return;
				
				it1 = itChann->second.modes.find(it->first);
				found = it1->second.find("+O");
				if (found == std::string::npos)
				{
					std::string str = mode;
					str.replace(0, 1, "+");
					it1 = itChann->second.modes.find(it->first);
					if (it1 != itChann->second.modes.end())
					{ 
						found = it1->second.find(str);
						if (found != std::string::npos)
						{
							std::string rem = it1->second;
							rem.erase(found, 2);;
							if (rem.size() > 0)
							{
								itChann->second.modes.erase(it->first);
								itChann->second.modes.insert(std::pair<int , std::string>(it->first, rem));
								it1 = itChann->second.modes.find(it->first);
							}
							else
								itChann->second.modes.erase(it->first);
						}
					}
					for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
					{
						sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "\n");
					}
				}
				ft_free(chan);
				return ;
			}
			else if (mode == "-b")
			{
				if (fds[fd].fd == it->first)
					return;
				std::string str = mode;
				str.replace(0, 1, "+");
				it1 = itChann->second.modes.find(it->first);
				found = it1->second.find("+O");
				if (found == std::string::npos)
				{
					if (it1 != itChann->second.modes.end())
					{
						found = it1->second.find(str);
						if (found != std::string::npos)
						{
							std::string rem = it1->second;
							rem.erase(found, 2);
							if (rem.size() > 0)
							{
								itChann->second.modes.erase(it->first);
								itChann->second.modes.insert(std::pair<int , std::string>(it->first, rem));
								itChann->second.listBan.erase(it->first);
							}
							else
							{
								itChann->second.modes.erase(it->first);
								itChann->second.listBan.erase(it->first);
							}
						}
					}
					for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
					{
						sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " MODE "+ itChann->first + " " + mode + " " + size + "!*@*\n");
					}
				}
				ft_free(chan);
				return ;
			}
		}
		else if (checkmode(mode) == 2)
		{
			mode_Arr_Three(chan, fd);
			return ;
		}
	}
}

void	server::add_op_chan(std::string input, int fd)
{
	std::string enough = ":localhost 461 " + myClient[fds[fd].fd].getNick() + " MODE :Not enough parameters\n";
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan || !chan[1])
	{
		ft_free(chan);
		return ;
	}
	std::string name = chan[1];
	if (lenArr(chan) == 1 || lenArr(chan) == 2 || lenArr(chan) >= 3)
	{
		if(lenArr(chan) == 1)
		{
			sendMsg(fds[fd].fd, enough);
			ft_free(chan);
			return ;
		}
		else if (lenArr(chan) == 2)
		{
			std::string namech = chan[1];
			if (namech.size() == 1 && namech.c_str()[0] == '#')
			{
				sendMsg(fds[fd].fd, enough);
				ft_free(chan);
				return ;
			}
		}
	}
	if (lenArr(chan) >= 4)
		mode_Arr_four(chan, fd);
	else if (lenArr(chan) == 3)
		mode_Arr_Three(chan , fd);
	else if (lenArr(chan) == 2)
		mode_Arr_Two(chan, fd);
}

void server::kick_chan(std::string input, int fd)
{
	char **chan = ft_split(input.c_str(), ' ');
	if (!chan || !chan[1])
	{
		ft_free(chan);
		return ;
	}
	if(lenArr(chan) < 3 )
	{
		if (chan[2] && chan[2][0] != ':')
		{
			ft_free(chan);
			sendMsg(fds[fd].fd, ":localhost 461 " + myClient[fds[fd].fd].getNick() + " KICK :Not enough parameters\n");
			return ;
		}
		ft_free(chan);
		sendMsg(fds[fd].fd, ":localhost 461 " + myClient[fds[fd].fd].getNick() + " KICK :Not enough parameters\n");
		return ;
	}
	else if (lenArr(chan) == 3 && chan[2][0] == ':')
	{
		ft_free(chan);
		sendMsg(fds[fd].fd, ":localhost 461 " + myClient[fds[fd].fd].getNick() + " KICK :Not enough parameters\n");
		return ;
	}
	// kick parsing
	std::string name = chan[1];
	std::string user = chan[2];
	size_t found;
	std::map<std::string , Channel>::iterator itChann;
	std::map<int, std::string>::iterator it1;
	std::map<int, Client>::iterator it;
	for (itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->first == name)
			break;
	}
	if (itChann == channels.end())
	{
		sendMsg(fds[fd].fd, ":localhost 403 " + myClient[fds[fd].fd].getNick()+ " " + name +  " :No such channel\n");
		ft_free(chan);
		return ;
	}
	it1 = itChann->second.modes.find(fds[fd].fd);
	if (it1 == itChann->second.modes.end())
	{
		sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + name + " :You're not channel operator\n");
		ft_free(chan);
		return ;
	}
	found = it1->second.find("+O");
	if (found == std::string::npos)
	{
		found = it1->second.find("+o");
		if (found == std::string::npos)
		{
			sendMsg(fds[fd].fd, ":localhost 482 " + myClient[fds[fd].fd].getNick() + " " + name + " :You're not channel operator\n");
			ft_free(chan);
			return ;
		}
	}
	it = itChann->second.usersChann.find(fds[fd].fd);
	if (it == itChann->second.usersChann.end())
	{
		sendMsg(fds[fd].fd, ":localhost 442 "+ myClient[fds[fd].fd].getNick() + " "+ name + " :You're not on that channel\n");
		ft_free(chan);
		return ;
	}
	for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
	{
		if (it->second.getNick() == user)
			break;
	}
	if (it == itChann->second.usersChann.end())
	{
		sendMsg(fds[fd].fd, ":localhost 401 "+ myClient[fds[fd].fd].getNick()+ " " + user +" :No such nick/channel\n");
		sendMsg(fds[fd].fd, ":localhost 441 " + myClient[fds[fd].fd].getNick()+ " " + user + " " + name + " :They aren't on that channel\n");
		ft_free(chan);
		return ;
	}
	if (it->first == fds[fd].fd)
	{
		ft_free(chan);
		return ;
	}
	int id = it->first;
	Client c = it->second;
	for(it = itChann->second.usersChann.begin(); it != itChann->second.usersChann.end(); it++)
	{
		sendMsg(it->first, ":" + myClient[fds[fd].fd].getNick() + " KICK "+ name + " " + c.getNick() + ":" + myClient[fds[fd].fd].getNick() + "\n");
	}
	itChann->second.modes.erase(id);
	itChann->second.usersChann.erase(id);
	ft_free(chan);
	return ;
}

void	server::Check_invite(std::string input, int i)
{
	std::string auterror = ":localhost 451 * INVITE :You must finish connecting with another nickname first.\n";
	if (!myClient[fds[i].fd].getAuth()) {
		sendMsg(fds[i].fd, auterror);
		return ;
	}
	input = input.substr(6, input.length() - 6);
	char **data = ft_split(input.c_str(), ' ');
	if (lenArr(data) != 2) 
	{
		ft_free(data);
		sendMsg(fds[i].fd, ":localhost 461 " + myClient[fds[i].fd].getNick() + " INVITE :Not enough parameters\n");
		return ;
	}
	std::string target = data[0];
	std::string namechan = data[1];
	ft_free(data);
	if (myClient[fds[i].fd].getNick() == target)
	{
		sendMsg(fds[i].fd, ":localhost 443 " + myClient[fds[i].fd].getNick() + " " + target + " " + namechan + " :is already on channel\n");
		return ;
	}
	std::map<int, Client>::iterator it;
	int target_fd = 0;
	for (it = myClient.begin(); it != myClient.end(); it++)
	{
		if (it->second.getNick() == target)
		{
			target_fd = it->first;
			break ;
		}
	}
	if (it == myClient.end())
	{
		sendMsg(fds[i].fd, ":localhost 401 "+ myClient[fds[i].fd].getNick()+ " " + target +" :No such nick/channel\n");
		return;
	}	
	std::map<std::string , Channel>::iterator itChann;
	std::map<int, std::string>::iterator it1;
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->second.getName() == namechan)
		{
			for (it = itChann->second.usersChann.begin() ; it != itChann->second.usersChann.end(); it++)
			{	
				if (it->second.getNick() == target)
				{
					sendMsg(fds[i].fd, ":localhost 443 " + myClient[fds[i].fd].getNick() + " " + target + " " + namechan + " :is already on channel\n");
					return ;
				}
			}
		}
	}
	size_t found;
	for(itChann = channels.begin(); itChann != channels.end(); itChann++)
	{
		if (itChann->first == namechan)
		{
			break;	
		}
	}
	if (itChann == channels.end())
	{
		sendMsg(fds[i].fd, ":localhost 403 " + myClient[fds[i].fd].getNick()+ " " + namechan +  " :No such channel\n");
			return ;
	}
	it = itChann->second.usersChann.find(fds[i].fd);
	if (it == itChann->second.usersChann.end())
	{
		sendMsg(fds[i].fd, ":localhost 442 "+ myClient[fds[i].fd].getNick() + " "+ namechan + " :You're not on that channel\n");
		return ;
	}
	it1 = itChann->second.modes.find(fds[i].fd);
	if (it1 == itChann->second.modes.end())
	{
		sendMsg(fds[i].fd, ":localhost 482 " + myClient[fds[i].fd].getNick() + " " + namechan + " :You're not channel operator\n");
		return ;
	}
	found = it1->second.find("+O");
	if (found == std::string::npos)
	{
		found = it1->second.find("+o");
		if (found == std::string::npos)
		{
			sendMsg(fds[i].fd, ":localhost 482 " + myClient[fds[i].fd].getNick() + " " + namechan + " :You're not channel operator\n");
			return ;
		}
	}
	sendMsg(fds[i].fd, ":localhost 341 " + myClient[fds[i].fd].getNick() + " " + target + " " + namechan + "\n");
	sendMsg(fds[i].fd, ":localhost NOTICE @" + namechan + " :"+ myClient[fds[i].fd].getNick() + " invited " + target + " into channel " + namechan + "\n");
	sendMsg(target_fd, ":" + myClient[fds[i].fd].getNick() + " INVITE " + target + " :" + namechan + "\n");
	itChann->second.invite_list.insert(std::pair<int, Client>(target_fd, it->second));

}

void	server::Parse_cmd(std::string input, int i)
{
	std::string	message;
	std::cout << input << std::endl;
	message = ":localhost 421 "+ input + " :Unknown command\n";
	if (!input.compare(0, 4, "PASS"))
		Check_pass(input, password, i);
	else if (!(input.compare(0, 4, "NICK")) )
		Check_nick(input, i);
	else if (!(input.compare(0, 4, "USER")))
		Check_user(input, i);
	else if (!(input.compare(0, 4, "QUIT")))
		Check_quit(i);
	else if (!(input.compare(0, 5, "ADMIN")))
		Check_admin(i);
	else if (!(input.compare(0, 4, "TIME")))
		Check_time(i);
	else if (!(input.compare(0, 3, "WHO")))
		Check_who(input, i);
	else if (!(input.compare(0, 7, "PRIVMSG")))
		Check_privmsg(input, i);
	else if (!(input.compare(0, 6, "NOTICE")))
		Check_notice(input, i);
	else if (!(input.compare(0, 3, "BOT")))
		CreateBot(myClient, input, fds[i].fd);
	else if (!(input.compare(0, 4, "JOIN")))
		Check_join(input, i);
	else if (!(input.compare(0, 5, "NAMES")))
		check_users(input, i);
	else if (!(input.compare(0 , 4, "PART")))
		check_quit_chan(input, i);
	else if (!(input.compare(0, 4, "MODE")))
		add_op_chan(input, i);
	else if (!(input.compare(0, 4, "KICK")))
		kick_chan(input, i);
	else if (!(input.compare(0, 6, "INVITE")))
		Check_invite(input, i);
	else
		sendMsg(fds[i].fd, message);
}