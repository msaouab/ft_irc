
# include "../Includes/Client.hpp"

Client::Client() {
}
Client::Client(std::string pass, std::string nick, std::string user) {
	this->pass = pass;
	this->nick = nick;
	this->user = user;
}
Client::~Client() {
}

void	Client::setPass(std::string _pass) {
	this->pass = _pass;
}
void	Client::setNick(std::string _nick) {
	this->nick = _nick;
}
void	Client::setUser(std::string _user) {
	this->user = _user;
}

std::string	Client::getPass(void) const {
	return (this->pass);
}
std::string	Client::getNick(void) const {
	return (this->nick);
}
std::string	Client::getUser(void) const {
	return (this->user);
}
