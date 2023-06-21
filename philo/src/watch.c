/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/21 03:31:00 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

bool	watch_them_die(struct s_simulation *sim)
{
	struct s_table		*table;
	uint32_t			i;
	uint32_t			x;

	table = sim->table;
	while (true)
	{
		i = 0;
		x = 0;
		while (i < sim->philosophers)
		{
			if (table->times_eaten >= sim->must_eat)
				x++;
			table = table->next;
			i++;
		}
		msg_print(sim->printer);
		if (x == sim->philosophers && sim->must_eat >= 0)
		{
			printf("Everyone has eaten %d times\n", sim->must_eat);
			exit (EXIT_SUCCESS);
		}
		table = sim->table;
	}
	return (EXIT_FAILURE);
}
