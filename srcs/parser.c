#include "ft_ping.h"

int		check_option(char c)
{
	int		i;
	static char	opt[NB_OPT] = {'6', 'v', 'h', 'D', 'c', 's', 'i'};

	i = 0;
	while (i < NB_OPT)
	{
		if (opt[i] == c)
		{
			printf("option %c trouve\n", c);
			return (i);
		}
		i++;
	}
	printf("ft_ping invalid option -- '%c'\n", c);
	return (-1);
}

t_info		bad_value(int idx, t_info arg)
{
	static char	*err_msg[2] = {"number of packet to transmit.", "timing interval"};

	printf("ft_ping: bad %s\n", err_msg[idx]);
	arg.idx = -1;
	return (arg);
}

int		get_value(char *str, void *dst)
{
	int	i;
	int	*value;

	i = 0;
	value = dst;
	*value = 0;
	printf("Hello dec\n");
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
	while (*str != '\0')
	{
		if (*str == '.' && point == 0)
			point++;
		else if (*str < '0' || *str > '9')
			return (-1);
		else if (fraq < 6)
		{
			if (point == 1)
			{
				fraq++;
				dec *= 10;
				tmp = (*str - '0');
				tmp /= dec;
				*value += tmp;
			}
			else
				*value = *value * 10 + (*str - '0');
		}
		i++;
		str++;
	}
	if (i == 0)
		return (-1);
	return (0);
}

/*
**	has_value is set to 1 if options takes a value
**	exemple : ping -c 25 localhost1`			
*//*
t_info		fill_struct(t_info arg, char *option, char *next)
{
	int		i;
	int		*arg_addr;
	static int	size[NB_OPT] = {0, 1, 2, 3, 4, 6, 8};
	static int	has_value[NB_OPT] = {0, 0, 0, 0, 1, 1, 1};

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
			if (has_value[arg.idx] == 1)
			{
				if ((*(arg_addr + size[arg.idx] + 1) = get_value((option + i + 1))) == -1)
				{
					arg.pars_idx++;
					if ((*(arg_addr + size[arg.idx] + 1) = get_value(next)) == -1)
						return (bad_value(arg.idx, arg));
				}
				return (arg);
			}
			i++;
		}
	}
	else
		arg.idx = -1;
	return (arg);
}*/

t_info		fill_struct(t_info arg, char *option, char *next)
{
	int		i;
	int		*arg_addr;
	static int	size[NB_OPT] = {0, 1, 2, 3, 4, 5, 6};
	static int	has_value[NB_OPT] = {0, 0, 0, 0, 1, 1, 2};
	static int	(*find_val[2])(char *value, void *dst) = {&get_value, &get_float};

	arg_addr = (int *)&arg;
	printf("option = %s\n", option);
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
				//printf("On a une value a %d et l'arg vaut %d\n", has_value[arg.idx], find_val[has_value[arg.idx] - 1](next));
				if (/*(*(arg_addr + size[arg.idx]) = */find_val[has_value[arg.idx] - 1](option + i + 1, (void *)(arg_addr + size[arg.idx])) == -1)//get_value((option + i + 1))) == -1)
				{
					printf("On check l'arg d'apres\n");
					arg.pars_idx++;
					if (/*(*(arg_addr + size[arg.idx]) = */find_val[has_value[arg.idx] - 1](next, (void *)(arg_addr + size[arg.idx])) == -1)//get_value(next)) == -1)
						return (bad_value(arg.idx, arg));
				}
				else
				{
					printf("WTF\n");
					return (arg);
				}
			}
			i++;
		}
	}
	else
		arg.idx = -1;
	return (arg);
}

t_info		parser(int ac, char **av)
{
	t_info		arg;

	bzero(&arg, sizeof(t_info));
	arg.interval = 1;
	arg.size = 56;
	arg.count = 5;
	arg.pars_idx = 1;
	printf("6: %d\nh: %d\nv: %d\nD: %d\nc: %d\ni: %f\ns: %d\n", arg.format, arg.h, arg.v, arg.d, arg.count, arg.interval, arg.size);
	while (arg.pars_idx < ac - 1)
	{
		printf("%d %d\n", arg.pars_idx, ac);
		arg = fill_struct(arg, av[arg.pars_idx], av[arg.pars_idx + 1]);
		arg.pars_idx++;
		if (arg.idx == -1)
			break ;
	}
	if (arg.pars_idx >= ac)
	{
		usage(av[0]);
		arg.idx = -1;
		return (arg);
	}
	printf("6: %d\nh: %d\nv: %d\nD: %d\nc: %d\ni: %f\ns: %d\n", arg.format, arg.h, arg.v, arg.d, arg.count, arg.interval, arg.size);
	printf("Host = %s\n", av[arg.pars_idx]);
	arg.host = av[arg.pars_idx];
	return (arg);
}
