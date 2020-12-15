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

int		get_idx_char(char *str, char c)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			printf("i = %d\n", i);
			return (i);
		}
		i++;
	}
	if (str[i] == '\0')
		return (1);
	return (-1);
}

int		is_ipv4(char *addr, int count)
{
	int		i;
	int		max;

	i = 0;
	printf("Salut %s\n", addr);
	if ((max = get_idx_char(addr, '.')) <= 3 && count <= 3)
		count ++;
	else 
	{
		printf("count = %d max = %d\n", count, addr);
		return (0);
	}
	while (i < max)
	{
		if (addr[i] < '0' || addr[i] > '9')
			return 0;
		i++;
	}
	if (addr[i] == '\0')
		return (1);
	return (is_ipv4(addr + i + 1, count));
}

struct sockaddr_in 	lookup_host(char *host, int *error)
{
	struct addrinfo		*res;
	struct addrinfo		hints;
	struct sockaddr_in	dst;

	memset(&dst, sizeof(struct sockaddr_in), 0);
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	if (getaddrinfo(host, NULL, &hints, &res) != 0)
	{
		*error = 1;
		return (dst);
	}
	dst = (*(struct sockaddr_in *)res->ai_addr);
	return (dst);
}

struct sockaddr_in 	get_ip_addr(char *host, int *error)
{
	struct sockaddr_in	dst;
	struct in_addr		addr;

	dst.sin_family = AF_INET;
	//printf("size in %d size %d\n", sizeof(struct sockaddr_in), sizeof(struct sockaddr));
	if (is_ipv4(host, 0) == 1)
	{
	//	printf("get addr from %s\n", host);
		if (inet_pton(AF_INET, host, &addr) == 1)
			dst.sin_addr = addr;
	//	printf("after = %s\n", inet_ntoa(addr));
	}
	else
	{
	//	printf("vroom\n");
		return (lookup_host(host, error));
	}
	return (dst);
}
/*
int		lookup_host(char *host)
{
	int			i;
	struct addrinfo		hints, *res;
	int			errcode;
	char			addrstr[100];
	struct hostent		*hostinfo = NULL;
	
	i = 0;

	printf("ip = %d\n", is_ipv4(host, 0));
	return (0);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;

	errcode = getaddrinfo(host, NULL, &hints, &res);
	if (errcode != 0)
	{
		perror("getaddrinfo\n");
		return (-1);
	}
	printf("host = %s\n", host);
	while (res)
	{
		inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, 100);
		//printf("%d: addrstr = %s\n", i, addrstr);
		switch (res->ai_family)
		{
			case AF_INET:
				ptr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
				break;
			case AF_INET6:
				printf("Inet6\n");
				ptr = &((struct sockaddr_in6 *)res->ai_addr)->sin6_addr;
				break;
		}
		inet_ntop(res->ai_family, ptr, addrstr, 100);
		//if (res->ai_family != PF_INET6)
			printf("%d: IPv%d address: %s (%s)\n", i, res->ai_family == PF_INET6 ? 6 : 4, addrstr, res->ai_canonname);
		//ft_ping(*res, hostinfo, addrstr, 56);
		res = res->ai_next;
		i++;
	}
	return (0);
}*/

struct ip	set_ip(struct sockaddr_in dst, int size)
{
	struct ip ip_hdr;

	inet_pton(AF_INET, "10.0.2.15", &ip_hdr.ip_src);
	memcpy(&ip_hdr.ip_dst, &dst.sin_addr, sizeof(struct in_addr));
//	printf("SOURCE : %s\n", inet_ntoa(ip_hdr.ip_src));
//	printf("DEST : %s\n",  inet_ntoa(ip_hdr.ip_dst));	
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

struct icmp	set_icmp(void)
{
	struct icmp		icmp_hdr;

	icmp_hdr.icmp_type = ICMP_ECHO;
	icmp_hdr.icmp_code = 0;
	icmp_hdr.icmp_id = 123;
	icmp_hdr.icmp_seq = 0;
	return (icmp_hdr);
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
	//icmp = set_icmp();
	//memcpy(buffer + sizeof(struct ip), &icmp, sizeof(struct icmp));
	//printf("size = %d\n", sizeof(struct icmp));
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
	//printf("ans = %d\n", ans);
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
	//printf("diff = %d, min = %d et max = %d\n", diff, stat.time.min, stat.time.max);
	if (stat.time.min > diff || stat.time.min == 0)
		stat.time.min = diff;
	if (stat.time.max < diff)
		stat.time.max = diff;
	if (stat.lst == NULL)
	{
	//	printf("Init\n");
		stat.lst = (t_time_list *)malloc(sizeof(t_time_list) * 1);
		stat.lst->val = diff;
		stat.lst->next = NULL;
	}
	else
	{
	//	printf("add new membre\n");
		new_member(diff);
	}
	stat.time.ave += diff;
	printf("%d.%d ms\n", diff / 1000, diff % 1000);
}

int		get_micro(float delay, int unit)
{
	int	i;
	int	res;

	res = 0;
	while ((delay = delay - (long)delay) > 0)
	{
		delay *= 10;
		res = res * 10 + delay;
		i++;
		if (i == 6)
			break ;
	}
	return (res);
}

int		diff(int micro, int *secs)
{
	if (micro > 999999)
	{
	 	*secs++;
		return (micro - 999999);
	}
	return (micro);
}

void		ft_wait(float delay)
{
	int			sec;
	int			usec;
	struct timeval		tv;
	struct timeval		tv2;
	
	if (delay == 0)
		return ;
	gettimeofday(&tv, NULL);
	gettimeofday(&tv2, NULL);
	sec = (int)(delay);
	usec = diff(tv.tv_usec + get_micro(delay, sec), &sec);
	while (tv2.tv_sec < tv.tv_sec + sec)
		gettimeofday(&tv2, NULL);
	while (tv2.tv_usec < usec)
		gettimeofday(&tv2, NULL);
}

int		ft_sqrt(double val)
{
	double A, B, M, XN;

	if (val == 0.0)
		return (0);
	M = 1;
	XN = val;
	while (XN >= 2.0)
	{
		XN = 0.25 * XN;
		M = 2.0 * M;
	}
	while (XN < 0.5)
	{
		XN = 4.0 * XN;
		M = 0.5 * M;
	}
	A = XN;
	B = 1.0 - XN;
	do {
		A = A * (1.0 + 0.5 * B);
		B = 0.25 * (3.0 + B) * B * B;
	} while (B >= 1.0E-15);
	return (A * M);
}

int		standart_div(void)
{
	int		div;
	long		sum;
	t_time_list	*tmp;

	sum = 0;
	tmp = stat.lst;
	while (tmp != NULL)
	{
		div = tmp->val - stat.time.ave / stat.nb_sent;
		//printf("val = %d div = %d\n", tmp->val, div);
		div *= div;
		sum += div;
		tmp = tmp->next;
	}
	sum /= stat.nb_sent;
	//printf("sum = %d\n", sum);
	return (ft_sqrt(sum));
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

void		ft_ping(struct sockaddr_in dst, t_info data)
{
	int			len;
	int			sock;
	char			*buffer;
	char			rec[255];
	struct ip		*ip;
	struct icmp		*icmp;
	struct timeval		tv;
	struct timeval		tv2;
	char			name[SIZE_IP4 + 1];

	memset(&rec, 0, 255);	
	struct icmp		*res;
	struct msghdr		msg;
	struct iovec		iov[1];
	struct sockaddr_storage src_addr;

	src_addr.ss_family = dst.sin_family;
	res = (struct icmp *)(rec + sizeof(struct ip));
	iov[0].iov_base = rec;
	iov[0].iov_len = 255;
	msg.msg_name = &src_addr;
	msg.msg_namelen = sizeof(struct sockaddr_storage);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = 0;
	msg.msg_controllen = 0;

	buffer = creat_payload(dst, data);
	ip = (struct ip *)buffer;
	icmp = (struct icmp *)(ip + 1);
	if ((sock = creat_socket()) < 0)
		return ;
	gettimeofday(&stat.init, NULL);
	printf("PING %s (%s) %d(%d) bytes of data\n", data.host, inet_ntop(AF_INET, &stat.addr, name, SIZE_IP4 + 1), data.size, data.size + sizeof(struct icmp));
	while (stat.nb_sent < data.count)
	{
		stat.nb_sent++;
		buffer = creat_payload(dst, data);
		ip = (struct ip *)buffer;
		icmp = (struct icmp *)(ip + 1);
		icmp->icmp_cksum = 0;
		ip->ip_sum = cksum((unsigned short *)buffer, ip->ip_hl);
		icmp->icmp_cksum = cksum((unsigned short *)icmp, SIZE - sizeof(struct icmp));
		len = SIZE;
		gettimeofday(&tv, NULL);
		sendto(sock, buffer, SIZE, 0, (struct sockaddr *)&dst, len);
			//recvmsg(sock, &msg, 0);
			//recvfrom(sock, buffer, sizeof(ip) + sizeof(icmp) + SIZE, 0, (struct sockaddr *)&dst, (socklen_t *)len);
			printf("FUNC A LA CON %d\n", read_msg(sock, &dst));
			gettimeofday(&tv2, NULL);
			/*for (int g = 0; g < 255; g++)
			{
				printf("%d ", res[g]);
				if (g != 0 && g % 8 == 0)
					printf("\n");
			}
			printf("-------------------------------------------------------------------------------------\n");
			for (int g = 0; g < 255; g++)
			{
				printf("%d ", buffer[g]);
				if (g != 0 && g % 8 == 0)
					printf("\n");

			}*/
			if (icmp->icmp_type == ICMP_ECHOREPLY)
			{	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=", data.size + sizeof(struct icmp) - sizeof(struct ip), inet_ntop(AF_INET, &stat.addr, name, 255), icmp->icmp_seq, ip->ip_ttl); 
				time_passed(tv, tv2);
				stat.nb_rec++;
			}
		icmp->icmp_seq++;
		ft_wait(data.interval);
	}
	gettimeofday(&stat.end, NULL);
	print_stat();
	close(sock);
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

void		sig_term(int code)
{
	printf("stats\n");
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

	signal(SIGTERM, &sig_term);
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
