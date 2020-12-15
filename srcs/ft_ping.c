/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:46:05 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/15 19:52:48 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#define SIZE 84

void		ft_ping(struct sockaddr_in dst, t_info data)
{
	int			len;
	int			sock;
	char			*buffer;
	char			rec[255];
	struct ip		*ip;
	struct icmp		*icmp;
	struct timeval		tv;
	struct timeval		tv2;
	char			name[SIZE_IP4 + 1];

	memset(&rec, 0, 255);	
	struct icmp		*res;
	struct msghdr		msg;
	struct iovec		iov[1];
	struct sockaddr_storage src_addr;

	src_addr.ss_family = dst.sin_family;
	res = (struct icmp *)(rec + sizeof(struct ip));
	iov[0].iov_base = rec;
	iov[0].iov_len = 255;
	msg.msg_name = &src_addr;
	msg.msg_namelen = sizeof(struct sockaddr_storage);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = 0;
	msg.msg_controllen = 0;

	buffer = creat_payload(dst, data);
	ip = (struct ip *)buffer;
	icmp = (struct icmp *)(ip + 1);
	if ((sock = creat_socket()) < 0)
		return ;
	gettimeofday(&stat.init, NULL);
	printf("PING %s (%s) %d(%d) bytes of data\n", data.host, inet_ntop(AF_INET, &stat.addr, name, SIZE_IP4 + 1), data.size, data.size + sizeof(struct icmp));
	while (stat.nb_sent < data.count)
	{
		stat.nb_sent++;
		printf("AVANT\n");
		buffer = creat_payload(dst, data);
		printf("APRES\n");
		ip = (struct ip *)buffer;
		icmp = (struct icmp *)(ip + 1);
		icmp->icmp_cksum = 0;
		ip->ip_sum = cksum((unsigned short *)buffer, ip->ip_hl);
		icmp->icmp_cksum = cksum((unsigned short *)icmp, SIZE - sizeof(struct icmp));
		len = SIZE;
		gettimeofday(&tv, NULL);
		sendto(sock, buffer, SIZE, 0, (struct sockaddr *)&dst, len);
			//recvmsg(sock, &msg, 0);
			//recvmsg(sock, &msg, 0);
			//recvfrom(sock, buffer, sizeof(ip) + sizeof(icmp) + SIZE, 0, (struct sockaddr *)&dst, (socklen_t *)len);
			printf("FUNC A LA CON %d\n", read_msg(sock, &dst));
			gettimeofday(&tv2, NULL);
			/*for (int g = 0; g < 255; g++)
			{
				printf("%d ", res[g]);
				if (g != 0 && g % 8 == 0)
					printf("\n");
			}
			printf("-------------------------------------------------------------------------------------\n");
			for (int g = 0; g < 255; g++)
			{
				printf("%d ", buffer[g]);
				if (g != 0 && g % 8 == 0)
					printf("\n");

			}*/
			if (icmp->icmp_type == ICMP_ECHOREPLY)
			{	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=", data.size + sizeof(struct icmp) - sizeof(struct ip), inet_ntop(AF_INET, &stat.addr, name, 255), icmp->icmp_seq, ip->ip_ttl); 
				time_passed(tv, tv2);
				stat.nb_rec++;
			}
		icmp->icmp_seq++;
		ft_wait(data.interval);
	}
	gettimeofday(&stat.end, NULL);
	print_stat();
	close(sock);
}
