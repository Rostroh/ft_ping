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

int		send_msg(int sock, struct sockaddr_in dst, t_info data, int *seq)
{
	char			*buffer;
	struct ip		*ip;
	struct icmp		*icmp;
	
	buffer = creat_payload(dst, data);
	ip = (struct ip *)buffer;
	icmp = (struct icmp *)(ip + 1);
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = stat.nb_sent - 1;
	*seq = icmp->icmp_seq;
	ip->ip_sum = cksum((unsigned short *)buffer, ip->ip_hl);
	icmp->icmp_cksum = cksum((unsigned short *)icmp, SIZE - sizeof(struct icmp));
	gettimeofday(&stat.tv1, NULL);
	sendto(sock, buffer, SIZE, 0, (struct sockaddr *)&dst, data.size + sizeof(struct icmp));
	return (data.size + sizeof(struct icmp));
}

void		ft_ping(struct sockaddr_in dst, t_info data)
{
	int			seq;
	int			sock;
	char			name[SIZE_IP4 + 1];

	if ((sock = creat_socket()) < 0)
		return ;
	gettimeofday(&stat.init, NULL);
	printf("PING %s (%s) %d(%d) bytes of data\n", data.host, \
			inet_ntop(AF_INET, &stat.addr, name, SIZE_IP4 + 1), \
			data.size, data.size + sizeof(struct icmp));
	while (stat.nb_sent < data.count || data.count == 0)
	{
		stat.nb_sent++;
		send_msg(sock, dst, data, &seq);
		if (read_msg(sock, &dst) > 0)
		{
			gettimeofday(&stat.tv2, NULL);
			printf("%d bytes from %s: icmp_seq=%d ttl=%d time=", \
	data.size + sizeof(struct icmp) - sizeof(struct ip), \
	inet_ntop(AF_INET, &stat.addr, name, 255), seq, data.ttl); 
			time_passed(stat.tv1, stat.tv2);
			stat.nb_rec++;
		}
		ft_wait(data.interval);
	}
	gettimeofday(&stat.end, NULL);
	print_stat();
	close(sock);
}
