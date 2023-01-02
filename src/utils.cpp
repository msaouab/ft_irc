# include "../Includes/ircserv.hpp"

bool isNumber(std::string str)
{
	int i = 0;
    while ( str[i]) {
        if (std::isdigit(str[i]) == 0)
            return false;
		i++;
    }
    return true;
}
