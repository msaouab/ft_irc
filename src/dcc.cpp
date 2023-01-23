// #include "../Includes/server.hpp"

// void 	server::dcc_accept(std::string input, int i)
// {
//     (void)input;
//     (void)i;
// }

// void 	server::dcc_send(std::string input, int i)
// {
//     (void)i;
//     input = input.substr(4, input.length() - 4);
//     char ** send = ft_split(input.c_str(), ' ');
// 	if (lenArr(send)  != 3) {
// 		ft_free(send);
// 		std::string message = "DCC: SEND: Parameters: <filename> <host> <port>.\n> ";
// 		sendMsg(fds[i].fd, message, RED);
// 		return ;
// 	}
//     std::string dcc_filename = send[0];
//     std::string dcc_host = send[1];
//     std::string dcc_port = send[2];
//     ft_free(send);
//     if (std::stoi(dcc_port,0,10) == this->getPort() || std::stoi(dcc_port,0,10) < 1025)
//     {
//         sendMsg(fds[i].fd, "DCC: SEND: WRONG PORT NUMBER\n> ", RED);
// 		return ;
//     }
//     std::map<int, Client>::iterator it;
//     for (it = myClient.begin(); it != myClient.end(); it++)
//     {
//         if (it->second.getNick() == dcc_host)
// 	    {
		    
//             std::string targetIP = it->second.getIP();
// 		    break;
// 	    }
//     }
// }

// void 	server::dcc_reject(std::string input, int i)
// {
//     (void)input;
//     (void)i;
// }