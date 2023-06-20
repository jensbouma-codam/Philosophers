/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/20 03:17:23 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	spend_time(long time)
{
	long	start;

	start = timestamp();
	while (timestamp() - start < time)
		usleep(1);
}

void	*philo_lifecycle(void *arg)
{
	struct s_table	*t;

	t = (struct s_table *)arg;
	while (true)
	{
		philo_eat_sleep(t, t->arg);
		t->state = THINKING;
		if (t->arg->must_eat != -2 && t->times_eaten == t->arg->must_eat)
			break ;
		if (t->id % 2 == 0)
			spend_time((t->arg->time_to_die - timestamp()) / 8);
		else
			spend_time((t->arg->time_to_die - timestamp()) / 4);
	}
	return (NULL);
}

bool	philo_eat_sleep(struct s_table *table, struct s_arg *a)
{
	if (pthread_mutex_lock(&table->l_fork->mutex) != 0)
		return (false);
	if (a->philosophers == 1
		|| pthread_mutex_lock(&table->r_fork->mutex) != 0)
		return (pthread_mutex_unlock(&table->l_fork->mutex), false);
	table->took_forks = true;
	printf("%lu %d has taken a fork\n", timestamp() / 1000, table->id);
	table->dead_date = timestamp() + a->time_to_die;
	table->state = EATING;
	printf("%lu %d is eating\n", timestamp() / 1000, table->id);
	spend_time(a->time_to_eat);
	table->times_eaten++;
	pthread_mutex_unlock(&table->l_fork->mutex);
	pthread_mutex_unlock(&table->r_fork->mutex);
	table->state = SLEEPING;
	printf("%lu %d is sleeping\n", timestamp() / 1000, table->id);
	spend_time(a->time_to_sleep);
	printf("%lu %d is thinking\n", timestamp() / 1000, table->id);
	return (true);
}
