/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:58:02 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/16 21:04:20 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <string.h>

#define BUF_SIZE 255

struct sockaddr_in	*is_host(struct sockaddr_in *addr)
{
	char			name[255];
	struct sockaddr_in	*res;

	res = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	inet_ntop(AF_INET, &addr->sin_addr, name, 255);
	if (strcmp(name, "127.0.0.1") == 0)
	{
		printf("MDR\n");
		inet_pton(AF_INET, "10.0.2.15", &res->sin_addr);
		return (res);
	}
	return (addr);
}

void		test(char *buffer)
{
	struct ip	*ip;
	struct icmp 	*icmp;

	ip = (struct ip *)buffer;
	icmp = (struct icmp *)(ip + 1);
	if (icmp->icmp_type == ICMP_ECHOREPLY)
		printf("FACILE\n");
	else
		printf("COMPLIQUE %d\n", icmp->icmp_type);
}

int		src_is_host(char *buf)
{
	char		name[255];
	struct ip	*ip_hdr;

	ip_hdr = (struct ip *)buf;
	inet_ntop(AF_INET, &ip_hdr->ip_src, name, 255);
	if (strcmp(name, "10.0.2.15") == 0)
		return (1);
	return (0);
}

int		read_msg(int sock, struct sockaddr_in *addr)
{
	char			buf[BUF_SIZE];
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t			n;
	struct cmgshdr	*cmptr;

	errno = 0;
	msg.msg_flags = 0;
	msg.msg_name = addr;
	msg.msg_namelen = sizeof(addr);
	iov[0].iov_base = buf;
	iov[0].iov_len = BUF_SIZE;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	if (((n = recvmsg(sock, &msg, 0))) < 0)
	{
		strerror(errno);
		printf("PLOP %d\n", errno);
		return (n);
	}
	if (*(buf + sizeof(struct ip)) == ICMP_ECHO && src_is_host(buf) == 1)
	{
		if (((n = recvmsg(sock, &msg, 0))) >= 0)
		{
			test(buf);
			return (n);
		}
	}
	if (*(buf + sizeof(struct ip)) == ICMP_ECHOREPLY)
		return (n);
	else
		return (-1);
}
