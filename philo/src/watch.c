/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/10 20:53:09 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

void	watch_their_dead(struct s_table *seat, struct s_simulation *sim)
{
	// const long	ts = timestamp();
	if (seat->dead != 0 && timestamp() > seat->dead)
	{
		msg_add(sim->msg_queue, seat->id, "died");
		msg_print(sim->msg_queue);
		// if ((ts % 1000) >= 500)
		// 	printf("%ld %d %s\n", (ts / 1000) + 1, seat->id, "died");
		// else
		// 	printf("%ld %d %s\n", (ts / 1000), seat->id, "died");
		exit(EXIT_FAILURE);
	}
}

bool	watch_them_die(struct s_simulation *sim)
{
	struct s_table		*table;
	uint32_t			i;
	uint32_t			x;

	while (true)
	{
		i = 0;
		x = 0;
		table = sim->table;
		while (i < sim->philosophers)
		{
			if (!table->is_eating)
				watch_their_dead(table, sim);
			if (table->times_eaten >= sim->times_to_eat)
				++x;
			table = table->next;
			++i;
		}
		msg_print(sim->msg_queue);
		if (x == sim->philosophers && sim->times_to_eat >= 0)
		{
			printf("Everyone has eaten %d times\n", sim->times_to_eat);
			break ;
		}
		// usleep(5000);
	}
	return (EXIT_SUCCESS);
}
