/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 19:30:26 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/19 19:31:47 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

long	timestamp(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		error_exit("gettimeofday failed");
	return ((time.tv_sec * 1000000 + time.tv_usec));
}
