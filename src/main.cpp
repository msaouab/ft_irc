
#include "../Includes/ircserv.hpp"

int	main(int ac, char **av)
{
	if (ac != 3) {
		std::cout << "\nYou need to respect this Format: '";
		std::cout << GRAY << "./ircserv <port> <password>";
		std::cout << ED << "'.\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!isNumber(av[1])) {
		std::cout << "\nERROR: " << GRAY;
		std::cout << "ENTER THE CORRECT PORT.\n" << ED << std::endl;
	}
	// try {
	// }
	// catch(const std::exception& e) {
	// 	std::cerr << e.what() << std::endl;
	// }
}
