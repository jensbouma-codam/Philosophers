/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   table.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 20:27:40 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/21 04:21:00 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	table_mutex_init(struct s_simulation *sim)
{
	struct s_table	*table;

	table = sim->table;
	while (table)
	{
		if (pthread_mutex_init(&table->l_fork->mutex, NULL) != 0)
			error_exit("\n mutex init failed\n");
		table->l_fork->in_use = false;
		if (sim->philosophers != 1
			&& pthread_mutex_init(&table->r_fork->mutex, NULL) != 0)
			error_exit("\n mutex init failed\n");
		table->r_fork->in_use = false;
		table = table->next;
	}
}

struct s_table	*table_cutlery(int i)
{
	struct s_table	*table;
	struct s_table	*new;
	struct s_fork	*first_fork;

	table = NULL;
	first_fork = mem_add(1, sizeof(struct s_fork));
	while (i > 0)
	{
		new = mem_add(1, sizeof(struct s_table));
		if (i == 1)
			new->l_fork = first_fork;
		else
			new->l_fork = mem_add(1, sizeof(struct s_fork));
		new->next = NULL;
		if (table != NULL)
		{
			new->r_fork = table->l_fork;
			new->next = table;
		}
		else
			new->r_fork = first_fork;
		table = new;
		--i;
	}
	return (table);
}

void	table_tread_create(struct s_simulation *sim)
{
	struct s_table		*t;
	struct s_lifecycle	*l;
	uint32_t			i;

	i = sim->philosophers;
	t = sim->table;
	while (i)
	{
		if (pthread_create(&(t->philosopher), NULL, &philo_lifecycle, sim))
			error_exit("pthread_create() error");
		if (pthread_detach(t->philosopher) != 0)
			error_exit("pthread_detach() error");
		t = t->next;
		--i;
	}
}

void	table_join(struct s_simulation *sim)
{
	uint32_t		i;
	struct s_table	*t;

	i = sim->philosophers;
	t = sim->table;
	while (i > 0)
	{
		t->id = i;
		t->seat_taken = false;
		t = t->next;
		--i;
	}
}
