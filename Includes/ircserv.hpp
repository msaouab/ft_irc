
#ifndef _IRC_SERV_HPP__
# define _IRC_SERV_HPP__

# define GRAY "\033[1;2m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define ED "\033[0m"
# define BOLD "\x1b[1m"
# define ED_B "\x1b[0m"

# include <iostream>
# include <stdlib.h>
# include <algorithm>
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <map>

# include "./server.hpp"
# include "./Client.hpp"

// struct sockaddr_in	address;

bool	isNumber(std::string str);

#endif /* _IRC_SERV_HPP__ */
