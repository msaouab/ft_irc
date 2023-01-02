
#include "../Includes/ircserv.hpp"

int	main(int ac, char **av)
{
	if (ac != 3) {
		std::cout << "\nYou need to respect this Format: '";
		std::cout << GRAY << "./ircserv <port> <password>";
		std::cout << ED << "'.\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string port = av[1];
	std::string password = av[2];
	if (!isNumber(av[1]) || !port.size() || !password.size())
	 {
		std::cout << "\nERROR: " << GRAY;
		std::cout << "ENTER THE CORRECT PORT AND PASSWORD.\n" << ED << std::endl;
		exit(EXIT_FAILURE);
	}
	server my_server;
	std::cout << my_server.getPassword() << std::endl; 
	try
	{
		my_server.setPort(stoi(port)); 
		my_server.setPassword(password);
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
