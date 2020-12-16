/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:30:39 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/16 21:16:07 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int					get_idx_char(char *str, char c)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			printf("i = %d\n", i);
			return (i);
		}
		i++;
	}
	if (str[i] == '\0')
		return (1);
	return (-1);
}

static int					is_ipv4(char *addr, int count)
{
	int		i;
	int		max;

	i = 0;
	printf("Salut %s\n", addr);
	if ((max = get_idx_char(addr, '.')) <= 3 && count <= 3)
		count++;
	else
	{
		printf("count = %d max = %d\n", count, addr);
		return (0);
	}
	while (i < max)
	{
		if (addr[i] < '0' || addr[i] > '9')
			return (0);
		i++;
	}
	if (addr[i] == '\0')
		return (1);
	return (is_ipv4(addr + i + 1, count));
}

static struct sockaddr_in	lookup_host(char *host, int *error)
{
	struct addrinfo		*res;
	struct addrinfo		hints;
	struct sockaddr_in	dst;

	memset(&dst, sizeof(struct sockaddr_in), 0);
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	if (getaddrinfo(host, NULL, &hints, &res) != 0)
	{
		*error = 1;
		return (dst);
	}
	dst = (*(struct sockaddr_in *)res->ai_addr);
	return (dst);
}

struct sockaddr_in			get_ip_addr(char *host, int *error)
{
	struct sockaddr_in	dst;
	struct in_addr		addr;

	dst.sin_family = AF_INET;
	if (is_ipv4(host, 0) == 1)
	{
		if (inet_pton(AF_INET, host, &addr) == 1)
			dst.sin_addr = addr;
	}
	else
		return (lookup_host(host, error));
	return (dst);
}
