
# include "../Includes/Client.hpp"

Client::Client() {
}

Client::~Client() {
}

std::string	Client::getPass() const {
	return (this->pass);
}
std::string	Client::getNick() const {
	return (this->nick);
}
std::string	Client::getUser() const {
	return (this->user);
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
