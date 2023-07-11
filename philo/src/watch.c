/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 16:07:16 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

static bool	is_dead(struct s_table *seat, struct s_simulation *sim)
{
	if (seat->dead != 0 && timestamp() > seat->dead)
	{
		msg_add(sim->msg_queue, seat->id, "died");
		return (true);
	}
	return (false);
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
		while (i++ < sim->philosophers)
		{
			if (!table->is_eating && is_dead(table, sim))
				return (false);
			if (table->times_eaten >= sim->times_to_eat)
				++x;
			table = table->next;
		}
		msg_print(sim->msg_queue);
		if (x == sim->philosophers && sim->times_to_eat >= 0)
		{
			printf("Everyone has eaten %d times\n", sim->times_to_eat);
			return (true);
		}
		usleep(1000);
	}
}
