#include "ft_ping.h"
/*
static struct ip	set_ip(struct sockaddr_in dst, int ttl)
{
	struct ip		ip_hdr;

	printf("debut setip\n");
	inet_pton(AF_INET, "10.0.2.15", &ip_hdr.ip_src);
	printf("after inetpton\n");
	ft_memcpy(&ip_hdr.ip_dst, &dst.sin_addr, sizeof(struct in_addr));
	printf("after memcpy\n");
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = 56;
	ip_hdr.ip_id = 321;
	ip_hdr.ip_off = 0;
	ip_hdr.ip_ttl = ttl;
	ip_hdr.ip_p = IPPROTO_ICMP;
	ip_hdr.ip_sum = 0;
	printf("after init\n");
	return (ip_hdr);
}*/

char				*creat_payload(struct sockaddr_in dst, t_info data)
{
	char			*buffer;
	struct ip		*ip;
	struct icmp		*icmp;

	buffer = (char *)malloc(sizeof(char) * data.size + sizeof(struct icmp));
	ft_memset(buffer, 0, data.size + sizeof(struct icmp));
	ip = (struct ip *)buffer;
	inet_pton(AF_INET, "10.0.2.15", &ip->ip_src);
	ft_memcpy(&ip->ip_dst, &dst.sin_addr, sizeof(struct in_addr));
	ip->ip_v = 4;
	ip->ip_hl = 5;
	ip->ip_tos = 0;
	ip->ip_len = 56;
	ip->ip_id = 321;
	ip->ip_off = 0;
	ip->ip_ttl = data.ttl;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	icmp = (struct icmp *)(buffer + sizeof(struct ip));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = 123;
	icmp->icmp_seq = 1;
	for (int i = sizeof(struct ip) + sizeof(struct icmp); i < data.size; i++)
		buffer[i] = 0;
	return (buffer);
}
