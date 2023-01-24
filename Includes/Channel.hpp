#pragma once

#include "./ircserv.hpp"

class Channel
{
    private:
        std::string name;
        std::string type;
        std::string password;
        std::string sujet;
    public:
        std::map<int, Client> usersChann; // for watch client of channel 
        std::map<int, Client> listBan; //list of users is bann 
        std::map<int, std::string> modes;//add modes
        std::map<int, Client> usersAcitve;
        Channel(std::string name);
        Channel(std::string name, std::string type, std::string pass);
        ~Channel();
        void setType(std::string type);
        void setPassword (std::string pass);
        std::string getType();
        std::string getName() ;
        std::string getPassword();
};