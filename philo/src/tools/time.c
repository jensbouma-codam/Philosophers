/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 15:34:30 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

long	timestamp(bool reset)
{
	static long		start = -1;
	struct timeval	time;
	if (start != -1)
	{
		gettimeofday(&time, NULL);
		return ((time.tv_sec * 1000000 + time.tv_usec) - start);
	}
	if (reset)
	{
		gettimeofday(&time, NULL);
		start = (time.tv_sec * 1000000 + time.tv_usec);
	}
	return (-1);
}

void	spend_time(t_sim *s, int time)
{
	const long	start = timestamp(false);
	(void)s;
	while (start > 0 && timestamp(false) < (start + time))
		usleep(500);
}
