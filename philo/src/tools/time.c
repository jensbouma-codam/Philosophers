/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/27 01:43:21 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(t_sim *s)
{
	static long		start = 0;
	struct timeval	time;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(&s->time_mutex);
	if (start == 0)
		start = (time.tv_sec * 1000 + time.tv_usec / 1000);
	pthread_mutex_unlock(&s->time_mutex);
	return ((time.tv_sec * 1000 + time.tv_usec / 1000) - start);
}

void	spend_time(t_sim *s, long time)
{
	const long	start = timestamp(s);

	while (timestamp(s) < (start + time))
		usleep(400);
}
