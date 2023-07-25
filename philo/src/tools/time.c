/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 17:16:30 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(t_sim *s)
{
	static long		start = 0;
	struct timeval	time;
	long			timestamp;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(&s->start_time_mutex);
	if (start == 0)
		start = (time.tv_sec * 1000000 + time.tv_usec);
	if (s->start_lock)
	{
		timestamp = (time.tv_sec * 1000000 + time.tv_usec) - start;
		pthread_mutex_unlock(&s->start_time_mutex);
		return (timestamp);
	}
	start = (time.tv_sec * 1000000 + time.tv_usec);
	pthread_mutex_unlock(&s->start_time_mutex);
	return (-1);
}

void	spend_time(t_sim *s, int time)
{
	const long	start = timestamp(s);

	while (start > 0 && timestamp(s) < (start + time))
		usleep(500);
}
