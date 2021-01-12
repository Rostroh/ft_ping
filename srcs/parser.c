#include "ft_ping.h"

int		check_option(char c)
{
	int		i;
	static char	opt[NB_OPT] = {'v', 'h', 'D', 'c', 't', 'i'};

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
	static char	*err_msg[2] = {"number of packet to transmit.", "timing interval"};

	printf("tamales = %d\n", idx);
	printf("ft_ping: bad %s\n", err_msg[idx]);
	arg.idx = -1;
	return (arg);
}

int		get_value(char *str, void *dst, int *idx)
{
	int	i;
	int	*value;

	i = 0;
	value = dst;
	*value = 0;
	if (!str)
		return (-1);
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			break ;
		else
			*value = *value * 10 + *str - '0';
		i++;
		str++;
	}
	if (i == 0)
		return (-1);
	return (0);
}

int		get_float(char *str, void *dst, int *idx)
{
	int	i;
	int	fraq;
	int	dec;
	int	point;
	float	tmp;
	float	*value;

	printf("Salut get_float\n");
	i = *idx;
	fraq = 0;
	value = dst;
	*value = 0;
	point = 0;
	dec = 1;
	if (!str)
		return (-1);
	printf("Arg = %s\n", str);
	while (str[*idx] != '\0')
	{
		if (str[*idx] == '.' && point == 0)
			point++;
		else if (str[*idx] < '0' || str[*idx] > '9')
			break ;
		else if (fraq < 6)
		{
			if (point == 1)
			{
				fraq++;
				dec *= 10;
				tmp = (str[*idx] - '0');
				tmp /= dec;
				*value += tmp;
			}
			else
				*value = *value * 10 + (str[*idx] - '0');
		}
		(*idx)++;
	}
	printf("%d %d\n", i, *idx);
	if (i == *idx)
		return (-1);
	return (0);
}

t_info		fill_struct(t_info arg, char *option, char *next)
{
	int		i;
	int		*arg_addr;
	static int	size[NB_OPT] = {0, 1, 2, 3, 4, 5};
	static int	has_value[NB_OPT] = {0, 0, 0, 2, 2, 2};
	static int	(*find_val[2])(char *value, void *dst, int *idx) = {&get_value, &get_float};

	arg_addr = (int *)&arg;
	if (option[0] == '-' && strlen(option) > 1)
	{
		i = 1;
		while (option[i] != '\0')
		{
			arg.idx = check_option(option[i]);
			if (arg.idx == -1)
				return (arg);
			*(arg_addr + size[arg.idx]) = 1;
			if (has_value[arg.idx] != 0)
			{
				i++;
				if (find_val[has_value[arg.idx] - 1](option, (void *)(arg_addr + size[arg.idx]), &i) == -1)
				{
					printf("On check l'arg d'apres\n");
					arg.pars_idx++;
					i = 0;
					if (find_val[has_value[arg.idx] - 1](next, (void *)(arg_addr + size[arg.idx]), &i) == -1)
					{
						printf("Eh salut\n");
						return (bad_value(arg.idx - 3, arg));
					}
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
	if ((int)arg->ttl > 255)
	{
		arg->idx = -1;
		printf("./ft_ping: bad Time to live\n");
	}
	if ((int)arg->count == 0)
		*arg = bad_value(0, *arg);
}

t_info		parser(int ac, char **av)
{
	t_info		arg;

	bzero(&arg, sizeof(t_info));
	arg.interval = 1;
	arg.size = 56;
	arg.count = 0;
	arg.ttl = 127;
	arg.pars_idx = 1;
	while (arg.pars_idx < ac)
	{
		if (av[arg.pars_idx][0] == '-' && strlen(av[arg.pars_idx]))
			arg = fill_struct(arg, av[arg.pars_idx], av[arg.pars_idx + 1]);
		else
			break ;
		arg.pars_idx++;
		if (arg.idx == -1)
			return (arg);
	}
	if (arg.pars_idx > ac)
	{
		usage(av[0]);
		arg.idx = -2;
		return (arg);
	}
	check_opt(&arg);
	arg.host = av[arg.pars_idx];
	return (arg);
}
