#include "../Includes/Channel.hpp"

Channel::Channel(std::string name)
{
    if (name.length() < 200)
        this->name = name;
}

Channel::Channel(std::string  name, std::string mode)
{
    if (name.length() < 200)
    {
        this->name = name;
        this->modeChan = mode;
    }
}

void Channel::setPassword(std::string pass)
{
    this->password = pass;
}

void Channel::setType(std::string type)
{
    this->type = type;    
}
void Channel::setModeChan(std::string mode)
{
    this->modeChan = mode;
}

void Channel::setSize(int size)
{
    this->sizeOfUsers = size;
}

std::string Channel::getPassword() const
{
    return (this->password);
}

std::string Channel::getName() const
{
    return (this->name);
}

std::string Channel::getType()  const
{
    return (this->type);
}
std::string Channel::getModeChan()
{
    return (this->modeChan);
}

int Channel::getSizeOfUsers() const
{
    return (this->sizeOfUsers);
}
Channel::~Channel()
{
}
