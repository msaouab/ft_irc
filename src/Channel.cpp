#include "../Includes/Channel.hpp"

Channel::Channel(std::string name)
{
    if (name.length() < 200)
        this->name = name;
    
    // this->sujet = sujet;
    // std::vector<std::string>::iterator it;
    
    // for (it = modes.begin(); it != modes.end(); it++)
    // {
    //     if (*it == "+p")
    //     {
    //         std::cout << "jkfhdjkshfdsh"
    //     }
    // }
}

Channel::Channel(std::string  name, std::string type, std::string pass)
{
    if (name.length() < 200)
    {
        this->name = name;
        this->type = type;
        if (this->type == "private")
            this->password = pass;
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

std::string Channel::getPassword()
{
    return (this->password);
}

std::string Channel::getName()
{
    return (this->name);
}

std::string Channel::getType() 
{
    return (this->type);
}

Channel::~Channel()
{
}
