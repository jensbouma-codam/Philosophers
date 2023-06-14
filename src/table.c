/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   table.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 20:27:40 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/14 21:54:29 by jbouma        ########   odam.nl         */
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

void	table_join(struct s_table *table, struct s_arg a)
{
	int				i;
	struct s_philo	*p;
	struct timeval	t;

	i = a.philosophers;
	while (i > 0)
	{
		p = mem_add(1, sizeof(struct s_philo));
		p->id = i;
		gettimeofday(&t, NULL);
		p->time_to_eat = t.tv_sec * 1000000 + t.tv_usec + a.time_to_eat;
		p->time_to_die = t.tv_sec * 1000000 + t.tv_usec + a.time_to_die;
		p->time_to_sleep = t.tv_sec * 1000000 + t.tv_usec + a.time_to_sleep;
		p->state = THINKING;
		table->philosopher = p;
		table = table->next;
		i--;
	}
}
