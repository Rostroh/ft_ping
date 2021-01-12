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
			return (i);
		i++;
	}
	if (str[i] == '\0')
		return (1);
	return (-1);
}

static struct sockaddr_in	lookup_host(char *host, int *error)
{
	char			name[255];
	struct addrinfo		*res;
	struct addrinfo		hints;
	struct sockaddr_in	dst;

	memset(&dst, sizeof(struct sockaddr_in), 0);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags |= AI_CANONNAME;
	if (getaddrinfo(host, NULL, &hints, &res) != 0)
	{
		*error = 1;
		return (dst);
	}
	dst = (*(struct sockaddr_in *)res->ai_addr);
	stat.is_host = 1;
	stat.dns = 1;
	stat.dns_name = res->ai_canonname;
	while (res != NULL)
	{
		getnameinfo((struct sockaddr *)&dst, sizeof(struct sockaddr), name, 255, NULL, 0, NI_NUMERICSERV);
		res = res->ai_next;
	}
	return (dst);
}

struct sockaddr_in			get_ip_addr(char *host, int *error)
{
	struct sockaddr_in	dst;
	struct in_addr		addr;

	dst.sin_family = AF_INET;
	if (inet_pton(AF_INET, host, &addr) == 1)
	{
		stat.dns = 0;
		stat.dns_name = host;
		dst.sin_addr = addr;
	}
	else
		return (lookup_host(host, error));
	return (dst);
}
