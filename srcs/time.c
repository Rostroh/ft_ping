#include "ft_ping.h"

static void	new_member(int diff)
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

void		time_passed(struct timeval t1, struct timeval t2)
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
