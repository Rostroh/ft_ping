#include "ft_ping.h"

int		ft_strlen(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void		*ft_memset(void *s, int c, size_t n)
{
	size_t		i;
	unsigned char	*ptr;

	i = 0;
	ptr = s;
	while (i < n)
	{
		ptr[i] = c;
		i++;
	}
	return (s);
}

void		*ft_memcpy(void *dst, void *src, size_t n)
{
	size_t		i;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	s1 = dst;
	s2 = src;
	while (i < n)
	{
		s1[i] = s2[i];
		i++;
	}
	return (dst);
}

int		ft_strcmp(char *s1, char *s2)
{
	while (*s1 != 0 && *s2 != 0)
	{
		if (*s1 != *s2)
			return ((unsigned char)(*s1) - (unsigned char)(*s2));
		s1++;
		s2++;
	}
	return ((unsigned char)(*s1) - (unsigned char)(*s2));
}

char		*ft_strdup(const char *src)
{
	int	i;
	int	len;
	char	*dst;

	i = 0;
	len = ft_strlen(src);
	if (!(dst = (char *)malloc(sizeof(char) * len + 1)))
		return (NULL);
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
