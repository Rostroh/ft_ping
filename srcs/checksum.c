#include "ft_ping.h"

unsigned short	cksum(unsigned short *addr, int len)
{
	int		nleft;
	int		sum;
	unsigned short 	*w;
	unsigned short	ans;

	nleft = len;
	w = addr;
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
