/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:46:05 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/16 20:49:00 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#define SIZE 84

static int		send_msg(int sock, struct sockaddr_in dst, \
		t_info data, int *seq)
{
	char			name[255];
	char			*buffer;
	struct ip		*ip;
	struct icmp		*icmp;

	stat.nb_sent++;
	buffer = creat_payload(dst, data);
	ip = (struct ip *)buffer;
	icmp = (struct icmp *)(ip + 1);
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = stat.nb_sent - 1;
	*seq = icmp->icmp_seq;
	ip->ip_sum = cksum((unsigned short *)buffer, ip->ip_hl);
	icmp->icmp_cksum = cksum((unsigned short *)icmp, \
			data.size);
	gettimeofday(&stat.tv1, NULL);
	if (sendto(sock, buffer, data.size + sizeof(struct icmp), 0, \
			(struct sockaddr *)&dst, data.size + sizeof(struct icmp)) < 0)
		printf("Send error\n");
	return (data.size + sizeof(struct icmp));
}

static void		print_info(t_info data)
{
	char		name[255];

	if (strcmp(data.host, "localhost") == 0)
		printf("PING localhost(localhost(::1) %d data bytes\n", (int)data.size);
	else if (stat.dns == 1)	
		printf("PING %s (%s) %d(%d) bytes of data\n", stat.dns_name, \
			inet_ntop(AF_INET, &stat.addr, name, 255), \
			(int)data.size, (int)(data.size) + sizeof(struct icmp));
	else
		printf("PING %s (%s) %d(%d) bytes of data\n", data.host, \
			inet_ntop(AF_INET, &stat.addr, name, 255), \
			(int)data.size, (int)(data.size) + sizeof(struct icmp));
}

static void		print_comm(struct sockaddr_in dst, t_info data, int seq)
{
	char		name[255];

	if (data.d == 1)
		printf("[%ld-%ld] ", stat.tv1.tv_sec, stat.tv1.tv_usec);
	printf("%d bytes from ", (int)(data.size) + sizeof(struct icmp) - sizeof(struct ip));
	if (strcmp(data.host, "localhost") == 0)
		printf("localhost (::1)");
	else if (stat.dns == 1)
	{
		getnameinfo((struct sockaddr *)&dst, sizeof(struct sockaddr), name, 255, NULL, 0, NI_NUMERICSERV);
		printf("%s", name);
	}
	else
		printf("%s", inet_ntop(AF_INET, &stat.addr, name, 255));
	printf(": icmp_seq=%d ttl=%d time=", seq, (int)data.ttl);
}

void			ft_ping(struct sockaddr_in dst, t_info data)
{
	int			seq;
	int			sock;
	char		name[255];

	if ((sock = creat_socket()) < 0)
		return ;
	print_info(data);
	gettimeofday(&stat.init, NULL);
	while (stat.nb_sent < (int)data.count || data.count == 0)
	{
		send_msg(sock, dst, data, &seq);
		if (read_msg(sock, &dst) > 0)
		{
			gettimeofday(&stat.tv2, NULL);
			print_comm(dst, data, seq);
			time_passed(stat.tv1, stat.tv2);
			stat.nb_rec++;
		}
		ft_wait(data.interval);
	}
	gettimeofday(&stat.end, NULL);
	print_stat();
	close(sock);
}
