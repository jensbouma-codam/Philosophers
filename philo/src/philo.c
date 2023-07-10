/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/10 23:21:08 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	philo_eat(struct s_table *p, struct s_simulation *sim)
{
	pthread_mutex_lock(&p->l_fork->mutex);
	pthread_mutex_lock(&p->r_fork->mutex);
	if (p->r_fork == p->l_fork || sim->philosophers < 1)
	{
		pthread_mutex_unlock(&p->l_fork->mutex);
		pthread_mutex_unlock(&p->r_fork->mutex);
		return (false);
	}

	msg_add(sim->msg_queue, p->id, "has taken a fork");
	msg_add(sim->msg_queue, p->id, "has taken a fork");
	msg_add(sim->msg_queue, p->id, "is eating");
	p->dead = timestamp() + sim->time_to_die;
	p->is_eating = true;
	p->times_eaten++;
	usleep(sim->time_to_eat);
	p->is_eating = false;
	pthread_mutex_unlock(&p->l_fork->mutex);
	pthread_mutex_unlock(&p->r_fork->mutex);
	return (true);
}

void	eatsleeprepeat(struct s_table *seat, struct s_simulation *sim)
{
	seat->times_eaten = 0;
	seat->dead = timestamp() + sim->time_to_die;
	while (true && (sim->times_to_eat > 0 || sim->times_to_eat == -2))
	{
		if (philo_eat(seat, sim))
		{
			if (seat->times_eaten == sim->times_to_eat)
				break ;
			msg_add(sim->msg_queue, seat->id, "is sleeping");
			usleep(sim->time_to_sleep);
			msg_add(sim->msg_queue, seat->id, "is thinking");
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
	eatsleeprepeat(seat, sim);
	return (NULL);
}

void	philo_join_table(struct s_simulation *sim)
{
	struct s_table		*t;
	uint32_t			i;

	i = 0;
	t = sim->table;
	while (++i <= sim->philosophers)
	{
		if (pthread_create(&(t->philosopher), NULL, &philo_lifecycle, sim))
			error_exit("pthread_create() error");
		if (pthread_detach(t->philosopher) != 0)
			error_exit("pthread_detach() error");
		t = t->next;
	}
}
