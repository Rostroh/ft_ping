#include "ft_ping.h"

int		creat_socket(void)
{
	int				val;
	int				sock;
	struct timeval	timeout;

	val = 1;
	timeout.tv_sec = 0;
	timeout.tv_usec = 10;
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		printf("Socket error\n");
		return (-1);
	}
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &val, sizeof(int)) < 0)
	{
		printf("setsockopt error\n");
		return (-1);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, \
				sizeof(struct timeval)) < 0)
	{
		printf("setsockopt timeout error\n");
		return (-1);
	}
	return (sock);
}
