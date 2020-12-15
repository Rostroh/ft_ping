#include "ft_ping.h"

/*#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>*/
#define SIZE 84
#define CTRL 8192
/*
void		ft_ping(struct addrinfo data, struct hostent *hostinf, char *addr, int size)
{
}*/

struct ip	set_ip(struct sockaddr_in dst, int size)
{
	struct ip ip_hdr;

	inet_pton(AF_INET, "10.0.2.15", &ip_hdr.ip_src);
	memcpy(&ip_hdr.ip_dst, &dst.sin_addr, sizeof(struct in_addr));
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = htons(size);
	ip_hdr.ip_id = htons(321);
	ip_hdr.ip_off = htons(0);
	ip_hdr.ip_ttl = 255;
	ip_hdr.ip_p = IPPROTO_ICMP;
	ip_hdr.ip_sum = 0;
	return (ip_hdr);
}

char		*creat_payload(struct sockaddr_in dst, t_info data)
{
	char			*buffer;
	struct ip		ip;
	struct icmp		*icmp;

	buffer = (char *)malloc(sizeof(char) * data.size);
	memset(buffer, 0, data.size);
	memset(&ip, 0, sizeof(struct ip));
	memset(&icmp, 0, sizeof(struct icmp));
	ip = set_ip(dst, data.size);
	memcpy(buffer, &ip, sizeof(struct ip));
	icmp = (struct icmp *)(buffer + sizeof(struct ip));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = 123;
	icmp->icmp_seq = 0;
	return (buffer);
}

int		creat_socket(void)
{
	int		val;
	int		sock;
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
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0)
	{
		printf("setsockopt timeout error\n");
		return (-1);
	}
	return (sock);
}

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

void		new_member(int diff)
{
	t_time_list	*new;
	t_time_list	*tmp;

	tmp = stat.lst;
	new = (t_time_list *)malloc(sizeof(t_time_list) * 1);
	new->val = diff;
	new->next = NULL;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

float		time_passed(struct timeval t1, struct timeval t2)
{
	int		diff;

	diff = t2.tv_usec - t1.tv_usec;
	diff = diff < 0 ? -diff : diff;
	if (stat.time.min > diff || stat.time.min == 0)
		stat.time.min = diff;
	if (stat.time.max < diff)
		stat.time.max = diff;
	if (stat.lst == NULL)
	{
		stat.lst = (t_time_list *)malloc(sizeof(t_time_list) * 1);
		stat.lst->val = diff;
		stat.lst->next = NULL;
	}
	else
		new_member(diff);
	stat.time.ave += diff;
	printf("%d.%d ms\n", diff / 1000, diff % 1000);
}

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

int		read_msg(int sock, struct sockaddr_in *addr)
{
	char		prout[255];
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t		n;
	struct cmgshdr	*cmptr;
	union {
		struct cmsghdr	cm;
		char		control[CMSG_SPACE(sizeof(struct in_addr))];
	} control_un;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	msg.msg_flags = 0;
	msg.msg_name = addr;
	msg.msg_namelen = sizeof(addr);
	iov[0].iov_base = prout;
	iov[0].iov_len = 255;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	if (((n = recvmsg(sock, &msg, 0))) < 0)
		return (n);
	if (*(prout + sizeof(struct ip)) == ICMP_ECHOREPLY)
		printf("OK OK\n");
	else
		printf("PAS DROLE\n");
	return (n);
}

void		usage(char *name)
{
		printf("Usage: %s [-vhD] [-c count] [-i interval]\n\t\t [-s packetsize] destination.\nUsage: ./ft_ping -6 [-vhD] [-c count] [-i interval]\n\t\t    [-s packetsize] destination\n", name);
}

void		sig_int(int code)
{
	print_stat();
	exit(-1);
}

void		init(void)
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

int		main(int argc, char **argv)
{
	t_info			data;
	struct sockaddr_in	dst;

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
		dst = get_ip_addr(argv[data.pars_idx], 0);
		stat.addr = dst.sin_addr;
		ft_ping(dst, data);
	}
	return (0);
}
