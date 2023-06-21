/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/21 03:32:25 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	busy(long dead, struct s_table *t, long time, struct s_msg *p)
{
	long	start;
	long	ts;

	start = timestamp();
	ts = start;
	while (ts - start < time)
	{
		ts = timestamp();
		if (timestamp() > dead)
		{
			msg_add(p, t->id, "died");
			msg_print(p);
			exit(EXIT_FAILURE);
		}
	}
}

bool	philo_eat(struct s_table *t, long dead, struct s_simulation *sim)
{
	if (pthread_mutex_lock(&t->l_fork->mutex) != 0)
		return (false);
	if (sim->philosophers == 1
		|| pthread_mutex_lock(&t->r_fork->mutex) != 0)
		return (pthread_mutex_unlock(&t->l_fork->mutex), false);
	msg_add(sim->printer, t->id, "has taken a fork");
	msg_add(sim->printer, t->id, "is eating");
	dead = timestamp() + sim->time_to_die;
	busy(dead, t, sim->time_to_eat, sim->printer);
	pthread_mutex_unlock(&t->l_fork->mutex);
	pthread_mutex_unlock(&t->r_fork->mutex);
	msg_add(sim->printer, t->id, "is sleeping");
	busy(dead, t, sim->time_to_sleep, sim->printer);
	msg_add(sim->printer, t->id, "is thinking");
	return (true);
}

void	esr(struct s_table *seat, long dead, struct s_simulation *sim)
{
	uint32_t			times_eaten;

	while (true && (sim->must_eat > 0 || sim->must_eat == -2))
	{
		if (philo_eat(seat, dead, sim))
			times_eaten++;
		else
		{
			msg_add(sim->printer, seat->id, "died");
			msg_print(sim->printer);
			exit(EXIT_FAILURE);
		}
		if (sim->must_eat != -2 && times_eaten == sim->must_eat)
		{
			seat->times_eaten = times_eaten;
			break ;
		}
	}
}

void	*philo_lifecycle(void *arg)
{
	struct s_simulation	*sim;
	struct s_table		*seat;
	long				dead;

	sim = (struct s_simulation *)arg;
	seat = sim->table;
	if (pthread_mutex_lock(&sim->mutex) != 0)
		error_exit("\n mutex init failed\n");
	while (seat->seat_taken == true)
		seat = seat->next;
	seat->seat_taken = true;
	dead = 0;
	if (pthread_mutex_unlock(&sim->mutex))
		error_exit("\n mutex init failed\n");
	dead = timestamp() + sim->time_to_die;
	esr(seat, dead, sim);
	return (NULL);
}
