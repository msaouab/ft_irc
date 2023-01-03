
#include "../Includes/ircserv.hpp"

int	main(int ac, char **av)
{
	server my_server;

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
	try
	{
		my_server.setPort(stoi(port));
		my_server.setPassword(password);
		my_server.start();
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	// std::cout << my_server.getPassword() << std::endl;
}
