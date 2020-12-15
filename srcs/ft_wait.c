/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh </var/mail/rostroh>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:48:06 by rostroh           #+#    #+#             */
/*   Updated: 2020/12/15 18:54:10 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	get_micro(float delay, int unit)
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

static int	diff(int micro, int *secs)
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
