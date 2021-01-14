#include "ft_ping.h"

void		usage(char *name)
{
	printf("Usage: %s [-vhD] [-c count] [-i interval]\n", name);
	printf("\t\t [-t ttl] destination.\n");
}

void		sig_int(int code)
{
	print_stat();
	exit(-1);
}
