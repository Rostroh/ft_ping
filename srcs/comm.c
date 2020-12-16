/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:58:02 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/15 19:58:13 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#define BUF_SIZE 255

int		read_msg(int sock, struct sockaddr_in *addr)
{
	char		buf[BUF_SIZE];
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t		n;
	struct cmgshdr	*cmptr;

	msg.msg_flags = 0;
	msg.msg_name = addr;
	msg.msg_namelen = sizeof(addr);
	iov[0].iov_base = buf;
	iov[0].iov_len = BUF_SIZE;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	if (((n = recvmsg(sock, &msg, 0))) < 0)
		return (n);
	if (*(buf + sizeof(struct ip)) == ICMP_ECHOREPLY)
		return (n);
	else
		return (-1);
}
