
#include "../Includes/server.hpp"

server::server() {
}

server::~server() {
}

int server::getPort() const
{
    return (this->port);
}

std::string server::getPassword() const
{
    return (this->password);
}

void server::setPort(int port)
{
    this->port = port;
}

void server::setPassword(std::string password)
{
    this->password = password;
}

void	server::start()
{
	while (true)
	{}
}