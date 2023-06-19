/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/19 21:52:27 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_lifecycle(void *arg)
{
	struct s_table	*t;

	t = (struct s_table *)arg;
	while (true)
	{
		philo_eat_sleep(t, t->arg);
		t->state = THINKING;
		if (t->arg->must_eat != 0 && t->times_eaten == t->arg->must_eat)
			return (NULL);
	}
	return (NULL);
}

bool	philo_eat_sleep(struct s_table *table, struct s_arg *a)
{
	if (pthread_mutex_lock(&table->l_fork->mutex) != 0)
		return (false);
	if (a->philosophers != 1
		&& pthread_mutex_lock(&table->r_fork->mutex) != 0)
		return (pthread_mutex_unlock(&table->l_fork->mutex), false);
	table->took_forks = true;
	table->dead_date = timestamp() + a->time_to_die;
	table->state = EATING;
	usleep(a->time_to_eat);
	pthread_mutex_unlock(&table->l_fork->mutex);
	pthread_mutex_unlock(&table->r_fork->mutex);
	table->state = SLEEPING;
	usleep(a->time_to_sleep);
	return (true);
}
