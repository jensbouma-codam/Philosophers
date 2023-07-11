/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   table.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 20:27:40 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 17:01:49 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initialize mutexes for forks.
 * 
 * @param sim 
 */
void	table_add_mutexes(struct s_simulation *sim)
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

/**
 * @brief Give each philosopher a seat at the table.
 * 
 * @param sim 
 */
void	table_add_seats(struct s_simulation *sim)
{
	uint32_t		i;
	struct s_table	*t;

	i = 0;
	t = sim->table;
	while (++i <= sim->philosophers)
	{
		t->id = i;
		t->seat_taken = false;
		t->is_eating = false;
		t = t->next;
	}
}

/**
 * @brief Create a table object with forks.
 * 
 * @param i Amount of philosophers
 * @return struct s_table* 
 */
struct s_table	*table_add_cutlery(int i)
{
	struct s_table	*table;
	struct s_table	*new;
	struct s_fork	*first_fork;

	table = NULL;
	first_fork = mem_add(1, sizeof(struct s_fork));
	while (i > 0)
	{
		new = mem_add(1, sizeof(struct s_table));
		if (i-- == 1)
			new->l_fork = first_fork;
		else
			new->l_fork = mem_add(1, sizeof(struct s_fork));
		if (!new || !first_fork || !new->l_fork)
			return (NULL);
		new->next = table;
		if (table != NULL)
			new->r_fork = table->l_fork;
		else
			new->r_fork = first_fork;
		table = new;
	}
	return (table);
}
