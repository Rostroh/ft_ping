#include "ft_ping.h"

static void		init(void)
{
	stat.nb_sent = 0;
	stat.nb_rec = 0;
	stat.init.tv_sec = 0;
	stat.init.tv_usec = 0;
	stat.end.tv_sec = 0;
	stat.end.tv_usec = 0;
	stat.time.min = 0;
	stat.time.max = 0;
	stat.time.ave = 0;
	stat.lst = NULL;
}

int				bad_host(char *prog_name, char *host)
{
	printf("%s: %s: No address associated with hostname\n", prog_name, host);
	return (0);
}

int				main(int argc, char **argv)
{
	int			err;
	t_info				data;
	struct sockaddr_in	dst;

	err = 0;
	signal(SIGINT, &sig_int);
	init();
	if (getuid() != 0)
	{
		printf("%s need root privilege\n", argv[0]);
		return (0);
	}
	if (argc == 1)
		usage(argv[0]);
	else
	{
		data = parser(argc, argv);
		if (data.idx == -1)
			return (0);
		dst = get_ip_addr(argv[data.pars_idx], &err);
		if (err == 1)
			return (bad_host(argv[0], argv[data.pars_idx]));
		stat.addr = dst.sin_addr;
		ft_ping(dst, data);
	}
	return (0);
}
