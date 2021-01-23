#include "ft_ping.h"

void		usage(char *name)
{
	printf("Usage: %s [-vhD] [-c count] [-i interval]\n", name);
	printf("\t\t [-t ttl] destination.\n");
}

void		sig_int(int code)
{
	code = 0;
	if (code == 0)
		print_stat();
	exit(-1);
}

void		free_list(void)
{
	t_time_list	*to_free;

	while (stat.lst)
	{
		to_free = stat.lst;
		stat.lst = stat.lst->next;
		free(to_free);
	}
}
