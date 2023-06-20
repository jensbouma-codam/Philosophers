/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   table.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 20:27:40 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/20 04:17:53 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	table_mutex_init(struct s_table *table, struct s_arg *a)
{
	while (table)
	{
		if (pthread_mutex_init(&table->l_fork->mutex, NULL) != 0)
			error_exit("\n mutex init failed\n");
		table->l_fork->in_use = false;
		if (a->philosophers != 1
			&& pthread_mutex_init(&table->r_fork->mutex, NULL) != 0)
			error_exit("\n mutex init failed\n");
		table->r_fork->in_use = false;
		table = table->next;
	}
}

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

void	table_tread_create(struct s_table *t)
{
	while (t)
	{
		if (pthread_create(&(t->philosopher), NULL, &philo_lifecycle, t))
			error_exit("pthread_create() error");
		if (pthread_detach(t->philosopher) != 0)
			error_exit("pthread_detach() error");
		t->dead_date = timestamp() + t->arg->time_to_die;
		t = t->next;
	}
}

void	table_join(struct s_table *table, struct s_arg *a)
{
	uint64_t		i;
	struct s_table	*t;

	i = a->philosophers;
	t = table;
	while (i > 0)
	{
		t->id = i;
		t->arg = a;
		t->state = JOINING;
		t->dead = false;
		t->times_eaten = 0;
		t->dead_date = 0;
		t->took_forks = false;
		t->old_state = JOINING;
		t = t->next;
		i--;
		printf(".");
	}
	// t = table;
}
