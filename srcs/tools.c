#include "ft_ping.h"

void		usage(char *name)
{
		printf("Usage: %s [-vhD] [-c count] [-i interval]\n\t\t [-s packetsize] destination.\nUsage: ./ft_ping -6 [-vhD] [-c count] [-i interval]\n\t\t    [-s packetsize] destination\n", name);
}

void		sig_int(int code)
{
	print_stat();
	exit(-1);
}
