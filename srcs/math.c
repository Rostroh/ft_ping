/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:02:43 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/15 19:42:32 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	ft_sqrt(double val)
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
