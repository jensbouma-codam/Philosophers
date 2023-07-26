/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/26 14:59:48 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(t_sim *s)
{
	static long		start = 0;
	struct timeval	time;
	long			timestamp;

	pthread_mutex_lock(&s->start_time_mutex);
	gettimeofday(&time, NULL);
	if (start == 0)
		start = (time.tv_sec * 1000000 + time.tv_usec);
	timestamp = (time.tv_sec * 1000000 + time.tv_usec) - start;
	pthread_mutex_unlock(&s->start_time_mutex);
	return (timestamp);
}

void	spend_time(t_sim *s, int time)
{
	const long	start = timestamp(s);

	while (start > 0 && timestamp(s) < (start + time))
		usleep(500);
}
