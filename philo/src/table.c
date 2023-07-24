/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   table.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 20:27:40 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/24 16:43:26 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Give each philosopher a seat at the table.
 * 
 * @param sim 
 */
static void	table_add_seats(struct s_table *t, uint32_t x)
{
	uint32_t		i;

	i = 0;
	while (++i <= x)
	{
		t->id = i;
		t->seat_taken = false;
		t->is_eating = false;
		if (pthread_mutex_init(&t->l_fork->mutex, NULL) != 0)
			errorlog("\n mutex init failed\n");
		t->l_fork->in_use = false;
		if (i != 1
			&& pthread_mutex_init(&t->r_fork->mutex, NULL) != 0)
			errorlog("\n mutex init failed\n");
		t->r_fork->in_use = false;
		t = t->next;
	}
}

/**
 * @brief Create a table object with forks.
 * 
 * @param i Amount of philosophers
 * @return struct s_table* 
 */
struct s_table	*table_prepare(int i)
{
	struct s_table	*table;
	struct s_table	*new;
	struct s_fork	*first_fork;
	const int		philos = i;

	table = NULL;
	first_fork = ft_calloc(1, sizeof(struct s_fork));
	while (i > 0)
	{
		new = ft_calloc(1, sizeof(struct s_table));
		if (i-- == 1)
			new->l_fork = first_fork;
		else
			new->l_fork = ft_calloc(1, sizeof(struct s_fork));
		if (!new || !first_fork || !new->l_fork)
			return (NULL);
		new->next = table;
		if (table != NULL)
			new->r_fork = table->l_fork;
		else
			new->r_fork = first_fork;
		table = new;
	}
	table_add_seats(table, philos);
	return (table);
}
