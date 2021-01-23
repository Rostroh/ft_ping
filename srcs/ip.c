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

static struct sockaddr_in	lookup_host(char *host, int *error)
{
	int			err = 0;
	char			name[255];
	struct addrinfo		*res;
	struct addrinfo		*to_free;
	struct addrinfo		hints;
	struct sockaddr_in	dst;

	ft_memset(&dst, sizeof(struct sockaddr_in), 0);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_CANONNAME;
	if ((err = getaddrinfo(host, NULL, &hints, &res)) != 0)
	{
		printf("./ft_ping: %s: No address associated with hostname (error code = %d)\n", host, err);
		*error = -1;
		return (dst);
	}
	dst = (*(struct sockaddr_in *)res->ai_addr);
	stat.is_host = 1;
	stat.dns = 1;
	stat.dns_name = ft_strdup(res->ai_canonname);
	while (res != NULL)
	{
		getnameinfo((struct sockaddr *)&dst, sizeof(struct sockaddr), name, 255, NULL, 0, NI_NUMERICSERV);
		to_free = res;
		stat.cname = ft_strdup(name);
		res = res->ai_next;
		free(to_free->ai_canonname);
		free(to_free);
	}
	return (dst);
}

char					*check_digit(char *host)
{
	int			i;
	struct in_addr		addr;

	i = 0;
	if (inet_pton(AF_INET, host, &addr) == 1)
		return (host);
	while (host[i])
	{
		if (host[i] == '0')
			i++;
		else if (host[i] >= '1' && host[i] <= '9')
			return (NULL);
		else
			return (host);
	}
	return ("127.0.0.1");
}

struct sockaddr_in			get_ip_addr(char *host, int *error)
{
	char			*hostname;
	struct sockaddr_in	dst;
	struct in_addr		addr;

	dst.sin_family = AF_INET;
	if ((hostname = check_digit(host)) == NULL)
	{
		printf("connect: invalid argument\n");
		exit(0);
	}
	else if (inet_pton(AF_INET, hostname, &addr) == 1)
	{
		stat.dns = 0;
		stat.dns_name = ft_strdup(host);
		dst.sin_addr = addr;
		return (dst);
	}
	else
		return (lookup_host(hostname, error));
	return (dst);
}
