#include "ft_ping.h"

unsigned short	cksum(unsigned short *addr, int len)
{
	int		nleft = len;
	int		sum;
	unsigned short 	*w = addr;
	unsigned short	ans;

	sum = 0;
	ans = 0;
	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}
	if (nleft == 1)
	{
		*(unsigned char *)(&ans) = *(unsigned char *)w;
		sum += ans;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	ans = ~sum;
	return (ans);
}
