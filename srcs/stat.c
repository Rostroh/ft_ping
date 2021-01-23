#include "ft_ping.h"

static void		print_rtt(int mdev)
{
	printf("rtt min/avg/max/mdev = %d.%d/%d.%d/%d.%d/%d.%d ms\n", \
		stat.time.min / 1000, stat.time.min % 1000, \
		stat.time.ave / stat.nb_sent / 1000, \
		stat.time.ave / stat.nb_sent % 1000, stat.time.max / 1000, \
		stat.time.max % 1000, mdev / 1000, mdev % 1000);
}

int			percent(int sent, int rec)
{
	return ((sent - rec) * 100 / sent);
}

void			print_stat(void)
{
	int			sec;
	int			usec;
	int			mdev;

	gettimeofday(&stat.end, NULL);
	printf("\n--- %s ping statistics ---\n", stat.dns_name);
	printf("%d packets transmitted, %d received, %d%% packet loss, time ",\
			stat.nb_sent, stat.nb_rec, percent(stat.nb_sent, stat.nb_rec));
	sec = stat.end.tv_sec - stat.init.tv_sec;
	usec = stat.end.tv_usec - stat.init.tv_usec;
	if (usec < 0)
	{
		sec--;
		usec = -usec;
	}
	printf("%d%d.%dms\n", sec, usec / 1000, usec % 1000);
	mdev = standart_div();
	print_rtt(mdev);
	free_list();
	if (stat.dns_name)
		free(stat.dns_name);
	if (stat.cname)
		free(stat.cname);
}
