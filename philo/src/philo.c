/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/21 12:03:01 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	busy(long dead, struct s_table *t, long time, struct s_msg *p)
{
	long	start;
	long	ts;

	start = timestamp();
	while (timestamp() - start < time)
	{
		if (timestamp() > dead)
		{
			msg_add(p, t->id, "died");
			msg_print(p);
			exit(EXIT_FAILURE);
		}
	}
}

bool	philo_eat(struct s_table *p, struct s_simulation *sim)
{
	if (sim->philosophers == 1)
		return (false);
	pthread_mutex_lock(&p->l_fork->mutex);
	msg_add(sim->printer, p->id, "has taken a fork");
	pthread_mutex_lock(&p->r_fork->mutex);
	msg_add(sim->printer, p->id, "has taken a fork");
	msg_add(sim->printer, p->id, "is eating");
	p->dead = timestamp() + sim->time_to_die;
	busy(p->dead, p, sim->time_to_eat, sim->printer);
	pthread_mutex_unlock(&p->l_fork->mutex);
	pthread_mutex_unlock(&p->r_fork->mutex);
	msg_add(sim->printer, p->id, "is sleeping");
	busy(p->dead, p, sim->time_to_sleep, sim->printer);
	msg_add(sim->printer, p->id, "is thinking");
	return (true);
}

void	esr(struct s_table *seat, struct s_simulation *sim)
{
	seat->times_eaten = 0;
	while (true && (sim->must_eat > 0 || sim->must_eat == -2))
	{
		if (philo_eat(seat, sim))
		{
			seat->times_eaten++;
			if (seat->times_eaten == sim->must_eat)
				break ;
		}
		else
		{
			msg_add(sim->printer, seat->id, "died");
			msg_print(sim->printer);
			exit(EXIT_FAILURE);
		}
	}
}

void	*philo_lifecycle(void *arg)
{
	struct s_simulation	*sim;
	struct s_table		*seat;

	sim = (struct s_simulation *)arg;
	seat = sim->table;
	if (pthread_mutex_lock(&sim->mutex) != 0)
		error_exit("\n mutex lock failed\n");
	while (seat->seat_taken == true)
		seat = seat->next;
	seat->seat_taken = true;
	if (pthread_mutex_unlock(&sim->mutex))
		error_exit("\n mutex unlock failed\n");
	esr(seat, sim);
	return (NULL);
}
