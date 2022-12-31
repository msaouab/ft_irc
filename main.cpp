#include "Includes/ircserv.hpp"

int	main(int ac, char **av)
{
	if (ac != 3) {
		std::cout << "\nYou need to respect this Format: '";
		std::cout << GRAY << "./ircserv <port> <password>";
		std::cout << ED << "'.\n" << std::endl;
	}
	try {
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
}
