/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/11 14:56:20 by jbouma        ########   odam.nl         */
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
