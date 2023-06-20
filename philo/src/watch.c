/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/20 03:20:03 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

static void	is_everybody_full(struct s_table *table, bool *everybody_is_full)
{
	struct s_table	*t;

	t = table;
	while (t)
	{
		if (t->times_eaten != t->arg->must_eat)
			*everybody_is_full = false;
		t = t->next;
	}
}

static void	print_state(struct s_table *t, long ts)
{
	if (t->state != EATING && t->state != JOINING && ts > t->dead_date)
	{
		printf("%lu %d died\n", ts / 1000, t->id);
		exit(EXIT_FAILURE);
	}
}

bool	watch_them_die(struct s_table *table)
{
	bool			everybody_is_full;
	struct s_table	*t;

	t = table;
	everybody_is_full = true;
	while (t)
	{
		print_state(t, timestamp());
		if (everybody_is_full)
			is_everybody_full(t, &everybody_is_full);
		t = t->next;
		if (t == NULL)
		{
			if (everybody_is_full)
			{
				printf("Everybody has eaten %i times\n", table->arg->must_eat);
				exit(EXIT_SUCCESS);
			}
			t = table;
			everybody_is_full = true;
		}
	}
	return (EXIT_FAILURE);
}
