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
	if (ft_strcmp(name, "127.0.0.1") == 0)
	{
		inet_pton(AF_INET, "10.0.2.15", &res->sin_addr);
		return (res);
	}
	return (addr);
}

int		src_is_host(char *buf)
{
	char		name[255];
	struct ip	*ip_hdr;

	ip_hdr = (struct ip *)buf;
	inet_ntop(AF_INET, &ip_hdr->ip_src, name, 255);
	if (ft_strcmp(name, "10.0.2.15") == 0)
		return (1);
	return (0);
}

int		check_src(char *buf)
{
	char		name[255];
	struct ip	*ip_hdr;
	struct icmp	*icmp_hdr;

	ip_hdr = (struct ip *)buf;
	icmp_hdr = (struct icmp *)(buf + sizeof(struct ip));
	inet_ntop(AF_INET, &ip_hdr->ip_src.s_addr, name, 255);
	if (ip_hdr->ip_src.s_addr == stat.addr.s_addr)
		return (0);
	if (icmp_hdr->icmp_type == ICMP_TIME_EXCEEDED)
	{
		printf("From %s icmp_seq=%d Time to live exceeded\n", name, stat.nb_sent);
		return (1);
	}
	return (-1);
}

int		read_msg(int sock, struct sockaddr_in *addr)
{
	char			buf[BUF_SIZE];
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t			n;
	struct cmgshdr	*cmptr;

	errno = 0;
	ft_memset(&buf, 0, BUF_SIZE);
	msg.msg_flags = 0;
	msg.msg_name = addr;
	msg.msg_namelen = sizeof(addr);
	iov[0].iov_base = buf;
	iov[0].iov_len = BUF_SIZE;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	if (((n = recvmsg(sock, &msg, 0))) < 0)
		return (n);
	if (src_is_host(buf) != 1 && check_src(buf) == -1)
		return (-1);
	if (*(buf + sizeof(struct ip)) == ICMP_ECHO && src_is_host(buf) == 1)
	{
		if (((n = recvmsg(sock, &msg, 0))) >= 0)
			return (n);
	}
	if (*(buf + sizeof(struct ip)) == ICMP_ECHOREPLY)
		return (n);
	else
		return (-1);
}
