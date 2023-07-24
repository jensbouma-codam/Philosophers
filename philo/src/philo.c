/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/24 17:04:56 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_spend_time(uint32_t time)
{
	const long	start = timestamp();

	while (timestamp() < start + time)
		usleep(1);
}

bool	philo_eat(struct s_table *p, struct s_simulation *sim)
{
	if (p->l_fork->in_use || p->r_fork->in_use || sim->philosophers < 1)
		return (false);
	pthread_mutex_lock(&p->l_fork->mutex);
	pthread_mutex_lock(&p->r_fork->mutex);
	p->l_fork->in_use = true;
	if (msg_add(sim->msg_queue, p->id, "has taken a fork"))
		return (false);
	p->r_fork->in_use = true;
	if (msg_add(sim->msg_queue, p->id, "has taken a fork"))
		return (false);
	if (msg_add(sim->msg_queue, p->id, "is eating"))
		return (false);
	pthread_mutex_lock(&p->mutex);
	p->dead = timestamp() + sim->time_to_die;
	p->is_eating = true;
	p->times_eaten++;
	pthread_mutex_unlock(&p->mutex);
	philo_spend_time(sim->time_to_eat);
	// pthread_mutex_lock(&p->mutex);
	p->is_eating = false;
	// pthread_mutex_unlock(&p->mutex);
	p->l_fork->in_use = false;
	p->r_fork->in_use = false;
	pthread_mutex_unlock(&p->l_fork->mutex);
	pthread_mutex_unlock(&p->r_fork->mutex);
	return (true);
}

static bool	eatsleeprepeat(struct s_table *seat, struct s_simulation *sim)
{
	seat->times_eaten = 0;
	seat->dead = sim->time_to_die;
	if (sim->times_to_eat == 0)
		return (false);
	while (true)
	{
		if (philo_eat(seat, sim))
		{
			if (seat->times_eaten == sim->times_to_eat)
				break ;
			if (msg_add(sim->msg_queue, seat->id, "is sleeping"))
				return (false);
			philo_spend_time(sim->time_to_sleep);
			if (msg_add(sim->msg_queue, seat->id, "is thinking"))
				return (false);
		}
	}
	return (false);
}

void	*philo_lifecycle(void *arg)
{
	struct s_simulation	*sim;
	struct s_table		*seat;

	sim = (struct s_simulation *)arg;
	seat = sim->table;
	
	while (seat->seat_taken == true)
		seat = seat->next;
	seat->seat_taken = true;
	if (!eatsleeprepeat(seat, sim))
		return (NULL);
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
		pthread_mutex_init(&t->mutex, NULL);
		if (pthread_create(&(t->philosopher), NULL, &philo_lifecycle, sim))
			errorlog("pthread_create() error");
		if (pthread_detach(t->philosopher) != 0)
			errorlog("pthread_detach() error");
		t = t->next;
	}
}
