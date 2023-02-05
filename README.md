# ft_irc
	Internet Relay Chat

# Desciption
 - Internet Relay Chator IRC is a text-based communication protocol on the Internet.It offers real-time messaging that can be either public or private. Users can exchangedirect messages and join group channels.IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network.2

# Summary:
	- This project is about creating your own IRC server.You will use an actual IRC client to connect to your server and test it.
	- Internet is ruled by solid standards protocols that allow connected computers to interactwith each other.
	- It’s always a good thing to know.Version: 6

## Mandatory part
 - We have to develop an IRC server in C++ 98.
 - The server must be capable of handling multiple clients at the same time and never hang.
 - Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
 - An IRC client must be able to connect to your server without encountering any error.
 - Communication between client and server has to be done via TCP/IP (v4).
 - Using the IRC client with our server must be similar to using it with any official IRC server. However, we only have to implement the following features:
	-	We must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using the IRC client.
	- All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
	- We must have operators and regular users.
	- Then, we have to implement the commands that are specific to operators.

## Bonus part
 - Here are the extra features we added to our IRC server so it looks even more like and actual IRC server:
	- Handle file transfer.
	- Add a bot.

### Socket();
- The socket(): function shall create an unbound socket in a communications domain, and return a file descriptor that can be used in later function calls that operate on sockets.

- The socket() function takes the following arguments:
	- domain: Specifies the communications domain in which a socket is to be created we use AF_INET "Address Family Internet" the socket can communicate using IP addresses 'IPv4'.
	- type: Specifies the type of socket, such as SOCK_STREAM for a reliable, stream-oriented connection or SOCK_DGRAM for a connectionless, datagram-oriented connection.
	- protocol: Specifies a particular protocol to be used with the socket. Specifying a protocol of 0 causes socket() to use an unspecified default protocol appropriate for the requested socket type.

### fcntl();
- fcntl(): is a system call in Unix-like operating systems that is used to manipulate file descriptors. The file descriptor is a unique integer assigned to a file or other input/output (I/O) resource when it is opened. The fcntl function allows a process to change the properties of an open file descriptor, such as making it non-blocking, setting its close-on-exec flag, or changing its access modes.

- The fcntl function takes three arguments:
	- The file descriptor to be modified.
	- The command to be executed, which is specified as a constant.
	- An optional argument that is specific to the command.
<!-- <div align="center"><img src="Images/Diagrame.png"></div> -->

# 💪 Contributers

- [Mohamed SAOUAB](https://github.com/msaouab)
	- [intra](https://profile.intra.42.fr/users/msaouab)
- [Ilyass QESSAM](https://github.com/iqessam)
	- [intra](https://profile.intra.42.fr/users/iqessam)
- [Youssef GBOURI](https://github.com/ygbouri)
	- [intra](https://profile.intra.42.fr/users/ygbouri)
	
# Resources
 - Internet Relay Chat Protocol: ([Irc](https://www.rfc-editor.org/rfc/rfc1459));
 - Internet Relay Chat: Architecture
 - Internet Relay Chat: Channel Management
 - Internet Relay Chat: Client Protocol
 - Internet Relay Chat: Server Protocol
 - About Socket: ([Socket in C++](https://www.geeksforgeeks.org/socket-programming-cc/));
 - What is a Socket?
 - Unix Socket - Network Addresses
 - Unix Socket - Core Functions
 - [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/#intro);
 - The UChicago χ-Projects
 - [Server creation by IBM](https://www.ibm.com/docs/en/i/7.4?topic=designs-using-poll-instead-select).
 - [sockaddr_in struct](https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html).
 - some helpful resources:
 	- https://ncona.com/2019/04/building-a-simple-server-with-cpp/
	- https://modern.ircdocs.horse/#message-format
	- https://ircv3.net/irc/
	- http://chi.cs.uchicago.edu/chirc/index.html
	- https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/ioctl.htm
	- https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/poll.htm
	- https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/ssocko.htm
	- https://www.ibm.com/docs/en/i/7.4?topic=ssw_ibm_i_74/apis/accept.htm
	- https://pubs.opengroup.org/onlinepubs/000095399/functions/setsockopt.html
	- https://ircv3.net/irc/
	- [irc cheat sheet](https://gist.github.com/xero/2d6e4b061b4ecbeb9f99)