
#include "../Includes/server.hpp"

void	Check_Pass(std::string input, std::string password)
{
	if(input.compare(5, password.length(), password))
    {
        std::cout << RED << "\nWRONG PASSWORD!!!\n" << ED << std::endl;
		return ;
    }
    else
    {
        std::cout << GREEN << "\nWELCOME TO OUR_IRC SERVER\n" << ED << std::endl;
		// WelcomeMSG();
        return;
    }
}

void	Check_Nick(std::string input, int n)
{
	std::map<int, std::string> myClient;
	std::string	nickname;

	input = input.substr(5, input.length());
	std::pair<std::map<int, std::string>::iterator,bool> ret;
	ret = myClient.insert(std::pair<int, std::string>(n, input));
	std::cout << ret.first->first << " ==> " << ret.first->second << '\n';
	if (ret.second == true) {
		std::cout << "element " << ret.first->first << " already existed";
		std::cout << " with a value of " << ret.first->second << '\n';
	}
	std::map<int, std::string>::iterator it = myClient.begin();
	for (it=myClient.begin(); it!=myClient.end(); ++it)
    	std::cout << it->first << " => " << it->second << '\n';
}

void	Parse_Cmd(std::string input, std::string password, int client)
{
	if (!input.compare(0, 4, "PASS"))
		Check_Pass(input, password);
	else if (!(input.compare(0, 4, "NICK")))
		Check_Nick(input, client - 3);
	// else if (!(input.compare(0, 4, "USER")))
	else
		std::cout << RED << "\nCommand not found\n" << ED << std::endl;
}
