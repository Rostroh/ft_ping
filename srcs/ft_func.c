#include "ft_ping.h"

int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void		*ft_memset(void *s, int c, size_t n)
{
	int		i;

	i = 0;
	while (i < n)
	{
		(unsigned char *)s[i] = c;
		i++;
	}
	return (s);
}

int		ft_strcmp(char *s1, char *s2)
{
	while (*s1 != 0 && *s2 != 0)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
