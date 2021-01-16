#include "ft_ping.h"

int		check_option(char c)
{
	int		i;
	static char	opt[NB_OPT] = {'h', 'v', 'D', 'c', 't', 'i'};

	i = 0;
	while (i < NB_OPT)
	{
		if (opt[i] == c)
			return (i);
		i++;
	}
	printf("ft_ping invalid option -- '%c'\n", c);
	return (-1);
}

t_info		bad_value(int idx, t_info arg)
{
	static char	*err_msg[3] = {"bad number of packet to transmit.", "ttl out of range", "bad timing interval"};

	printf("idx = %d\n", idx);
	printf("ft_ping: %s\n", err_msg[idx]);
	arg.idx = -1;
	return (arg);
}

int		get_float(char *str, void *dst)
{
	int	i;
	int	fraq;
	int	dec;
	int	point;
	float	tmp;
	float	*value;

	i = 0;
	fraq = 0;
	value = dst;
	*value = 0;
	point = 0;
	dec = 1;
	if (!str)
		return (-1);
	while (str[i] != '\0')
	{
		if (str[i] == '.' && point == 0)
			point++;
		else if (str[i] < '0' || str[i] > '9')
			break ;
		else if (fraq < 6)
		{
			if (point == 1)
			{
				fraq++;
				dec *= 10;
				tmp = (str[i] - '0');
				tmp /= dec;
				*value += tmp;
			}
			else
				*value = *value * 10 + (str[i] - '0');
		}
		i++;
	}
	if (i == 0)
		return (-1);
	return (0);
}

t_info		fill_struct(t_info arg, char *option, char *next)
{
	int		i;
	int		*arg_addr;
	static int	has_value[NB_OPT] = {0, 0, 0, 1, 1, 1};

	arg_addr = (int *)&arg;
	if (option[0] == '-' && ft_strlen(option) > 1)
	{
		i = 1;
		while (option[i] != '\0')
		{
			arg.idx = check_option(option[i]);
			if (arg.idx == -1)
				return (arg);
			*(arg_addr + arg.idx) = 1;
			if (has_value[arg.idx] != 0)
			{
				i++;
				if (option[i] != '\0')
				{
					if (get_float(option + i, (void *)(arg_addr + arg.idx)) == -1)
						return (bad_value(arg.idx - 3, arg));
					break ;
				}
				else
				{
					arg.pars_idx++;
					if (get_float(next, (void *)(arg_addr + arg.idx)) == -1)
						return (bad_value(arg.idx - 3, arg));
					break ;
				}
			}
			else
				i++;
		}
	}
	return (arg);
}

void		check_opt(t_info *arg)
{
       	if ((int)arg->ttl == 0)
	{
		arg->idx = -1;
		printf("./ft_ping: can't set unicast time-to-live: Invalid argument\n");
	}
	if ((int)arg->ttl > 255)
	{
		arg->idx = -1;
		printf("./ft_ping: ttl %d out of range\n", (short)arg->ttl);
	}
	if ((int)arg->count == 0)
		*arg = bad_value(0, *arg);
}

t_info		parser(int ac, char **av)
{
	t_info		arg;

	bzero(&arg, sizeof(t_info));
	arg.host = NULL;
	arg.interval = 1;
	arg.size = 56;
	arg.count = INFINITY;
	arg.ttl = 127;
	arg.pars_idx = 1;
	while (arg.pars_idx < ac)
	{
		if (av[arg.pars_idx][0] == '-' && strlen(av[arg.pars_idx]))
			arg = fill_struct(arg, av[arg.pars_idx], av[arg.pars_idx + 1]);
		else
			arg.host = av[arg.pars_idx];
		arg.pars_idx++;
		if (arg.idx == -1)
			return (arg);
	}
	if (arg.pars_idx > ac || arg.host == NULL)
	{
		usage(av[0]);
		arg.idx = -2;
		return (arg);
	}
	check_opt(&arg);
	return (arg);
}
