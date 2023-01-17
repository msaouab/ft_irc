# include "../Includes/Client.hpp"

// Client::Client() : Clientfd(0), nick(0), user(0) { }
Client::Client() {
	this->Auth = false;
	this->Clientfd = 0;
	this->nick = "";
	this->user = NULL;
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
void	Client::setUser(char **_user) {
	this->user = _user;
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

std::ostream& operator<< (std::ostream& os, const Client& client) {
	os << client.getNick();
	return (os);
}