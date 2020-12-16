#ifndef FT_PING_H
# define FT_PING_H

# include <math.h>
# include <errno.h>
# include <sys/time.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/in_systm.h>

#define NB_OPT 7
#define SIZE_IP4 12
#define SIZE_HDR 28
/*
typedef struct		s_param
{
	int		bol;
	int		val;
}			t_param;
*/

typedef struct 			s_time_list
{
	int			val;
	struct s_time_list	*next;
}				t_time_list;

typedef struct			s_timeinfo
{
	int			min;
	int			max;
	int			ave;
}				t_timeinfo;

typedef struct 			s_stats
{
	struct in_addr		addr;
	int			nb_sent;
	int			nb_rec;
	struct timeval		init;
	struct timeval		end;
	struct timeval		tv1;
	struct timeval		tv2;
	t_timeinfo		time;
	t_time_list		*lst;
}				t_stats;

t_stats stat;

/*
**	h stand for help, set to 1 for display
**	v stand for verbose, set to 1 for display
**	d stand for timestamp, set to 1 for display
**	format, set to 1 for IPv6, default is IPv4
**	count (number of packets), set to 1 if count is set, 0 otherwise
**	interval between packets, set like count
**	size of packets, set like count
**	idx, set to -1 if an error occured while parsing arguments. Otherwise, has the value of the index in check_option (see parser.c)
**	pars_idx is the index of the arguments
*/
typedef struct		s_info
{
	int		h;
	int		v;
	int		d;
	int		count;
	int		size;
	int		ttl;
	float		interval;
	int		idx;
	int		pars_idx;
	char		*host;
}			t_info;

/*
**	checksum.c
*/
unsigned short		cksum(unsigned short *addr, int len);

/*
**	comm.c
*/
int			read_msg(int sock, struct sockaddr_in *addr);

/*
**	ft_ping.c
*/
void			ft_ping(struct sockaddr_in dst, t_info data);

/*
**	ft_wait.c
*/
void			ft_wait(float delay);

/*
**	ip.c
*/
struct sockaddr_in	get_ip_addr(char *host, int *error);

/*
**	math.c
*/
int			standart_div(void);

/*
**	parser.c
*/
t_info			parser(int ac, char **argv);

/*
**	payload.c
*/
char			*creat_payload(struct sockaddr_in dst, t_info data);

/*
**	socket.c
*/
int			creat_socket(void);

/*
**	stat.c
*/
void			print_stat(void);

/*
**	time.c
*/
float			time_passed(struct timeval t1, struct timeval t2);

/*
**	tools.c
*/
void			usage(char *name);
void			sig_int(int code);
#endif
