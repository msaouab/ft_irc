#include "../Includes/server.hpp"

void password_check(std::string input, std::string password)
{
	if(input.compare(5,password.length(), password))
    {
        std::cout << "WRONG PASSWORD!!!" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "WELCOME TO IRC SERVER" << std::endl;
        return;
    }
}

void connection_registration(std::string input)
{
        if(!(input.compare(0,4,"NICK")))
        {
            std::size_t pos = input.find(" ");
            std::string nickname = input.substr(pos + 1);
            // nick_is_used(nickname);
        }
        // else if ({)
        // {
        //     /* code */
        // }
        
            

}


void parse_command(std::string input, std::string password)
{
    // Client client;
	if(!input.compare(0,4,"PASS"))
		password_check(input, password);
	else if(!(input.compare(0,4,"USER")) || !(input.compare(0,4,"NICK")) || !(input.compare(0,4,"PASS")) 
		 || !(input.compare(0,4,"QUIT")) || !(input.compare(0,5,"SQUIT")))
        connection_registration(input);
    // else if (!(input.compare(0,4,"OPER")) || !(input.compare(0,4,"PART")) || !(input.compare(0,4,"JOIN")) || !(input.compare(0,4,"MODE")) 
	// 	 || !(input.compare(0,5,"NAMES")) || !(input.compare(0,4,"MODE")) || !(input.compare(0,5,"TOPIC"))
	// 	 || !(input.compare(0,4,"LIST")) || !(input.compare(0,6,"INVITE")) || !(input.compare(0,4,"KICK")))
	// 	std::cout << "AND THE COMMAND IS A CHANNEL OPERATIONS" << std::endl;
	// else if(!(input.compare(0,5,"STATS")) || !(input.compare(0,5,"ADMIN")) || !(input.compare(0,4,"INFO")))
	// 	std::cout << "AND THE COMMAND IS A Server queries and command" << std::endl;
	// else if(!(input.compare(0,7,"PRIVMSG")) || !(input.compare(0,6," NOTICE")))
	// 	std::cout << "AND THE COMMAND IS A Sending messages command" << std::endl;

	
	// int l = input.length();
	// if (std::count(input.c_str(),input.c_str()+l, ' ') == 4)
	// {
	// 	std::cout << "GOOD!!!" << std::endl;
	// }
	else
		std::cout << "Command not found :" << std::endl; 

}