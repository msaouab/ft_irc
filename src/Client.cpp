
# include "../Includes/Client.hpp"

Client::Client() {
}

Client::~Client() {
}

std::string	Client::getUser() const {
	return (this->user);
}
void	Client::setUser(std::string _user) {
	this->user = _user;
}
