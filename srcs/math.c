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

int		standart_div(void)
{
	int		div;
	long		sum;
	t_time_list	*tmp;

	sum = 0;
	tmp = stat.lst;
	while (tmp != NULL)
	{
		div = tmp->val - (stat.time.ave / stat.nb_sent);
		div *= div;
		sum += div;
		tmp = tmp->next;
	}
	sum /= stat.nb_sent;
	return (sqrt(sum));
}
