# include "../Includes/Client.hpp"

Client::Client() : Clientfd(0), nick(0), user(0) { }
Client::Client( int fd ) : Clientfd(fd), nick(), user() {
}
Client &Client::operator=(const Client &rhs) {
	this->Clientfd = rhs.Clientfd;
	this->nick = rhs.nick;
	this->user = rhs.user;
	return (*this);
}
Client::~Client() {
}

void	Client::setClientfd(int _clientfd) {
	this->Clientfd = _clientfd;
}
void	Client::setNick(std::string _nick) {
	this->nick = _nick;
}

void	Client::setUser(std::string	_user) {
	this->user = _user;
}

int		Client::getClientfd(void) const {
	return (this->Clientfd);
}
std::string	Client::getNick(void) const {
	return (this->nick);
}
std::string	Client::getUser(void) const {
	return (this->user);
}

// std::ostream& operator<< (std::ostream& os, const Client& client) {
// 	os << client.getUser();
// 	return (os);
// }