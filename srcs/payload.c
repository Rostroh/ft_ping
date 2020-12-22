#include "ft_ping.h"

static struct ip	set_ip(struct sockaddr_in dst, int size)
{
	struct ip		ip_hdr;

	inet_pton(AF_INET, "10.0.2.15", &ip_hdr.ip_src);
	memcpy(&ip_hdr.ip_dst, &dst.sin_addr, sizeof(struct in_addr));
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = htons(size);
	ip_hdr.ip_id = htons(321);
	ip_hdr.ip_off = htons(0);
	ip_hdr.ip_ttl = 255;
	ip_hdr.ip_p = IPPROTO_ICMP;
	ip_hdr.ip_sum = 0;
	return (ip_hdr);
}

char				*creat_payload(struct sockaddr_in dst, t_info data)
{
	char			*buffer;
	struct ip		ip;
	struct icmp		*icmp;

	buffer = (char *)malloc(sizeof(char) * data.size + sizeof(struct icmp));
	memset(buffer, 0, data.size + sizeof(struct icmp));
	memset(&ip, 0, sizeof(struct ip));
	memset(&icmp, 0, sizeof(struct icmp));
	ip = set_ip(dst, data.size);
	memcpy(buffer, &ip, sizeof(struct ip));
	icmp = (struct icmp *)(buffer + sizeof(struct ip));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = 123;
	icmp->icmp_seq = 0;
	for (int i = sizeof(struct ip) + sizeof(struct icmp); i < data.size; i++)
		buffer[i] = 0;
	return (buffer);
}
