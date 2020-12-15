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

int		read_msg(int sock, struct sockaddr_in *addr)
{
	char		prout[255];
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t		n;
	struct cmgshdr	*cmptr;
	union {
		struct cmsghdr	cm;
		char		control[CMSG_SPACE(sizeof(struct in_addr))];
	} control_un;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	msg.msg_flags = 0;
	msg.msg_name = addr;
	msg.msg_namelen = sizeof(addr);
	iov[0].iov_base = prout;
	iov[0].iov_len = 255;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	if (((n = recvmsg(sock, &msg, 0))) < 0)
		return (n);
	if (*(prout + sizeof(struct ip)) == ICMP_ECHOREPLY)
		printf("OK OK\n");
	else
		printf("PAS DROLE\n");
	return (n);
}
