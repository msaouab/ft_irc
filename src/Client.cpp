# include "../Includes/Client.hpp"

// Client::Client() : Clientfd(0), nick(0), user(0) { }
Client::Client() {
	this->Auth = false;
	this->Clientfd = 0;
	this->nick = "";
	this->user = NULL;
	this->ip = "";
	this->time = 0;
}

Client &Client::operator=(const Client &rhs) {
	this->Clientfd = rhs.Clientfd;
	this->nick = rhs.nick;
	this->user = rhs.user;
	this->ip = rhs.ip;
	this->time = rhs.time;
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
void	Client::setUser(char **_user) {
	this->user = _user;
}
void	Client::setIP(std::string _ip) {
	this->ip = _ip;
}
void	Client::setTime(long _time) {
	this->time = _time;
}

int		Client::getClientfd(void) const {
	return (this->Clientfd);
}
std::string	Client::getNick(void) const {
	return (this->nick);
}
char**	Client::getUser(void) const {
	return (this->user);
}
bool	Client::getAuth(void) const {
	return (this->Auth);
}
std::string		Client::getIP() const {
	return (this->ip);
}
long		Client::getTime() const {
	return (this->time);
}

std::ostream& operator<< (std::ostream& os, const Client& client) {
	os << client.getNick();
	return (os);
}