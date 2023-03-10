# include "../Includes/Client.hpp"

Client::Client() {
	this->Auth = false;
	this->Log = false;
	this->Clientfd = 0;
	this->nick = "";
	this->user = "";
	this->realname = "";
	this->ip = "";
	this->time = 0;
}

Client &Client::operator=(const Client &rhs) {
	this->nick = rhs.nick;
	this->user = rhs.user;
	this->realname = rhs.realname;
	this->Clientfd = rhs.Clientfd;
	this->Auth = rhs.Auth;
	this->Log = rhs.Log;
	this->ip = rhs.ip;
	this->time = rhs.time;
	return (*this);
}

bool Client::operator==(const Client &rhs) {
	if (rhs.getClientfd() == this->getClientfd())
		return (true);
	return (false);
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
std::string		Client::getIP() const {
	return (this->ip);
}
long		Client::getTime() const {
	return (this->time);
}


void	Client::setJoinChan( bool _join)
{
	this->joinChan = _join;
}

bool	Client::getJoinChan(void) const{
	return (this->joinChan);
}

std::ostream& operator<< (std::ostream& os, const Client& client) {
	os << client.getNick();
	return (os);
}