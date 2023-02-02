#pragma once

#include "./ircserv.hpp"

class Channel
{
    private:
        std::string name;
        std::string type;
        std::string modeChan;
        std::string password;
        std::string sujet;
        int        sizeOfUsers;
    public:
        std::map<int, Client> usersChann; // for watch client of channel 
        std::map<int, Client> listBan; //list of users is bann 
        std::map<int, std::string> modes;//add modes
        std::map<int, Client> usersAcitve;
        std::map<int, Client> invite_list;//List of invited users
        Channel(std::string name);
        Channel(std::string name, std::string mode);
        ~Channel();
        void setType(std::string type);
        void setPassword (std::string pass);
        void setModeChan(std::string mode);
        void setSize(int size);
        std::string getType() const;
        std::string getName() const;
        std::string getPassword() const;
        std::string getModeChan() ;
        int getSizeOfUsers() const;

};