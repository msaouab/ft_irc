
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

int	lenArr(char **arr)
{
	int	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

void	ft_free(char **arr)
{
	for (int i = 0; i < lenArr(arr); i++)
		free (arr[i]);
	free (arr);
}