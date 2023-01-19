# include "../Includes/Client.hpp"

// Client::Client() : Clientfd(0), nick(0), user(0) { }
Client::Client() {
	this->Auth = false;
	this->Log = false;
	this->Clientfd = 0;
	this->nick = "";
	this->user = "";
	this->realname = "";
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
void	Client::setAuth(bool _Auth) {
	this->Auth = _Auth;
}

void	Client::setLog(bool _Log) {
	this->Log = _Log;
}

void	Client::setUser(std::string _user) {
	this->user = _user;
}

void	Client::setRealname(std::string _real) {
	this->realname = _real;
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

std::string	Client::getRealname(void) const {
	return (this->realname);
}

bool	Client::getAuth(void) const {
	return (this->Auth);
}

bool	Client::getLog(void) const {
	return (this->Log);
}

std::ostream& operator<< (std::ostream& os, const Client& client) {
	os << client.getNick();
	return (os);
}