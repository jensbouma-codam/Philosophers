/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/21 12:03:36 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

void	watch_their_dead(struct s_table *seat, struct s_simulation *sim)
{
	if (seat->dead != 0 && timestamp() > seat->dead)
	{
		msg_add(sim->printer, seat->id, "died");
		msg_print(sim->printer);
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
			watch_their_dead(table, sim);
			if (table->times_eaten >= sim->must_eat)
				++x;
			table = table->next;
			++i;
		}
		msg_print(sim->printer);
		if (x == sim->philosophers && sim->must_eat >= 0)
		{
			printf("Everyone has eaten %d times\n", sim->must_eat);
			break ;
		}
	}
	return (EXIT_SUCCESS);
}
