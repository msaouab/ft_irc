/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaouab <msaouab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 20:13:33 by msaouab           #+#    #+#             */
/*   Updated: 2023/01/16 20:13:42 by msaouab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ircserv.hpp"

static int	lenword(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (*s)
	{
		if (s[i] != c && i == 0)
		{
			i = 1;
			count++;
		}
		if (s[i] == c && i == 1)
			i = 0;
		s++;
	}
	return (count);
}

static int	intchr(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (s[i] == c && s[i])
		i++;
	while (s[i] && s[i] != c)
	{
		i++;
		count++;
	}
	return (count);
}

static void	free_tab(char **splt, int row)
{
	int	i;

	i = -1;
	while (i < row)
	{
		free(splt[i]);
		i++;
	}
	free (splt);
}

static char	*subsplt(const char *s, char c, int *l)
{
	char	*str;
	int		lchr;
	int		j;
	int		k;

	j = 0;
	while (s[*l] && s[*l] == c)
		(*l)++;
	j = *l;
	lchr = intchr(&s[j], c) + 1;
	str = (char *)malloc(sizeof(char) * lchr);
	if (!str)
		return (NULL);
	k = 0;
	while (k < lchr - 1)
		str[k++] = s[j++];
	str[k] = '\0';
	*l = j;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**splt;
	int		len;
	int		i;
	int		l;

	l = 0;
	if (s == NULL)
		return (NULL);
	len = lenword(s, c);
	splt = (char**)malloc(sizeof(char *) * (len + 1));
	if (!splt)
		return (NULL);
	i = 0;
	while (i < len)
	{
		splt[i] = subsplt(s, c, &l);
		if (!splt[i])
		{
			free_tab(splt, i);
			return (NULL);
		}	
		i++;
	}
	splt[i] = NULL;
	return (splt);
}
