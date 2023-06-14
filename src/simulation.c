/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   simulation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/14 22:19:03 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	simulation(struct s_table *table, struct s_arg a)
{
	struct timeval	time;
	int				dead;
	int				alive;

	dead = 0;
	alive = 0;

	debug("%-15s%d\n%-15s%d\n%-15s%d\n%-15s%d\n%-15s%d\n",
		"philosophers", a.philosophers,
		"time_to_die", a.time_to_die,
		"time_to_eat", a.time_to_eat,
		"time_to_sleep", a.time_to_sleep,
		"must_eat", a.must_eat);

	while (table)
	{
		printf("left fork: %p\n", table->l_fork);
		printf("seat: %d\n", table->id);
		printf("state: %d\n", table->state);
		printf("time_to_die: %lu\n", table->time_to_die);
		printf("right fork: %p\n", table->r_fork);
		gettimeofday(&time, NULL);
		if (table->time_to_die < time.tv_sec * 1000000 + time.tv_usec)
			dead++;
		else
			alive++;
		table = table->next;
	}

	printf("dead: %d\n", dead);
	printf("alive: %d\n", alive);
	return (0);
}
