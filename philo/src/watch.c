/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/20 01:35:24 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

void	print_state(struct s_table *t, long ts)
{
	if (t->state != EATING && t->state != JOINING && ts > t->dead_date)
	{
		printf("%lu %d died\n", ts / 1000, t->id);
		exit(EXIT_FAILURE);
	}
	if (t->took_forks)
	{
		printf("%lu %d has taken a fork\n", ts / 1000, t->id);
		t->took_forks = false;
	}
	if (t->state == t->old_state)
		return ;
	t->old_state = t->state;
	if (t->state == EATING)
	{
		printf("%lu %d is eating\n", ts / 1000, t->id);
		t->times_eaten++;
	}	
	else if (t->state == SLEEPING)
		printf("%lu %d is sleeping\n", ts / 1000, t->id);
	else if (t->state == THINKING)
		printf("%lu %d is thinking\n", ts / 1000, t->id);
}

static void	is_everybody_full(struct s_table *table, bool *everybody_is_full)
{
	struct s_table	*t;

	t = table;
	while (t)
	{
		if (t->times_eaten != t->arg->must_eat)
			*everybody_is_full = false;
		else
		{
			printf("Everybody has eaten %i times\n", t->arg->must_eat);
			exit(EXIT_SUCCESS);
		}
		t = t->next;
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
		if (everybody_is_full)
			is_everybody_full(table, &everybody_is_full);
		print_state(t, timestamp());
		t = t->next;
		if (t == NULL)
		{
			t = table;
			everybody_is_full = true;
		}
	}
	return (EXIT_FAILURE);
}
