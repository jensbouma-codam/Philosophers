/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   table.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 20:27:40 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/14 22:11:40 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct s_table	*table_cutlery(int i)
{
	struct s_table	*table;
	struct s_table	*node;
	struct s_fork	*first_fork;

	table = NULL;
	first_fork = mem_add(1, sizeof(struct s_fork));
	while (i > 0)
	{
		node = mem_add(1, sizeof(struct s_table));
		if (i == 1)
			node->l_fork = first_fork;
		else
			node->l_fork = mem_add(1, sizeof(struct s_fork));
		node->next = NULL;
		if (table != NULL)
		{
			node->r_fork = table->l_fork;
			node->next = table;
		}
		else
			node->r_fork = first_fork;
		table = node;
		i--;
	}
	return (table);
}

void	table_join(struct s_table *t, struct s_arg a)
{
	int				i;
	struct timeval	time;

	i = a.philosophers;
	while (i > 0)
	{
		t->id = i;
		gettimeofday(&time, NULL);
		t->time_to_die = time.tv_sec * 1000000 + time.tv_usec + a.time_to_die;
		t->state = THINKING;
		t->times_eaten = 0;
		t = t->next;
		i--;
	}
}
