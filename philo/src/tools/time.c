/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 02:28:08 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

long	timestamp(void)
{
	static long		start = 0;
	struct timeval	time;

	gettimeofday(&time, NULL);
	if (start == 0)
		start = (time.tv_sec * 1000000 + time.tv_usec);
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

void	spend_time(t_sim *s, int time)
{
	const long	start = timestamp();

	while (!s->everbody_has_eaten.get(&s->everbody_has_eaten)
		&& !s->someone_died.get(&s->someone_died) && timestamp() < start + time)
		usleep(100);
}
