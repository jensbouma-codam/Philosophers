/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   watch.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 19:48:47 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/19 22:32:18 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <ctype.h>

static void	print_state(struct s_table *t, enum e_state state, long ts)
{
	if (t->took_forks)
	{
		printf("%lu %d has taken a fork\n", ts / 1000, t->id);
		t->took_forks = false;
	}
	if (t->state != EATING && t->state != JOINING && ts > t->dead_date)
	{
		if (t->dead != true)
		{
			t->dead = true;
			printf("%lu %d died\n", ts / 1000, t->id);
		}
		return ;
	}
	if (t->state == t->old_state)
		return ;
	t->old_state = state;
	if (state == EATING)
	{
		printf("%lu %d is eating\n", ts / 1000, t->id);
		t->times_eaten++;
	}	
	else if (state == SLEEPING)
		printf("%lu %d is sleeping\n", ts / 1000, t->id);
	else if (state == THINKING)
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
			printf("Everybody is full\n");
			exit(EXIT_SUCCESS);
		}
		t = t->next;
	}
}

bool	watch_them_die(struct s_table *table)
{
	bool			everybody_is_full;
	struct s_table	*t;
	long			ts;

	t = table;
	everybody_is_full = true;
	while (t)
	{
		ts = timestamp();
		is_everybody_full(table, &everybody_is_full);
		print_state(t, t->state, ts);
		t = t->next;
		if (t == NULL)
		{
			t = table;
			everybody_is_full = true;
		}
	}
	return (EXIT_FAILURE);
}
