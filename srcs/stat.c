#include "ft_ping.h"

void		print_stat(void)
{
	int		sec;
	int		usec;
	int		mdev;
	char		name[SIZE_IP4 + 1];

	gettimeofday(&stat.end, NULL);
	printf("\n--- %s ping statistics ---\n%d packets transmitted, %d received, %d loss, time ", inet_ntop(AF_INET, &stat.addr, name, SIZE_IP4 + 1), stat.nb_sent, stat.nb_rec, stat.nb_sent - stat.nb_rec);
	sec = stat.end.tv_sec - stat.init.tv_sec;
	usec = stat.end.tv_usec - stat.init.tv_usec;
	if (usec < 0)
	{
		sec--;
		usec = -usec;
	}
	printf("%ds %d.%dms\n", sec, usec / 1000, usec % 1000);
	//printf("sec = %d sec2 = %d usec = %d\n", stat.init.tv_sec, stat.end.tv_sec, usec);
	mdev = standart_div();
	printf("rtt min/avg/max/mdev = %d.%d/%d.%d/%d.%d/%d.%d ms\n", stat.time.min / 1000, stat.time.min % 1000, stat.time.ave / stat.nb_sent / 1000, stat.time.ave / stat.nb_sent % 1000, stat.time.max / 1000, stat.time.max % 1000, mdev / 1000, mdev % 1000);
}