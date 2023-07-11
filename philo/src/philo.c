/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/19 20:06:17 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/11 13:40:39 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_spend_time(uint32_t time)
{
	const long	start = timestamp();

	while (timestamp() < start + time)
		usleep(100);
}

bool	philo_eat(struct s_table *p, struct s_simulation *sim)
{
	if (p->l_fork->in_use || p->r_fork->in_use || sim->philosophers < 1)
		return (false);
	pthread_mutex_lock(&p->l_fork->mutex);
	pthread_mutex_lock(&p->r_fork->mutex);
	p->l_fork->in_use = true;
	msg_add(sim->msg_queue, p->id, "has taken a fork");
	p->r_fork->in_use = true;
	msg_add(sim->msg_queue, p->id, "has taken a fork");
	msg_add(sim->msg_queue, p->id, "is eating");
	p->dead = timestamp() + sim->time_to_die + 1;
	p->is_eating = true;
	p->times_eaten++;
	philo_spend_time(sim->time_to_eat);
	p->is_eating = false;
	p->l_fork->in_use = false;
	p->r_fork->in_use = false;
	pthread_mutex_unlock(&p->l_fork->mutex);
	pthread_mutex_unlock(&p->r_fork->mutex);
	return (true);
}

void	eatsleeprepeat(struct s_table *seat, struct s_simulation *sim)
{
	seat->times_eaten = 0;
	seat->dead = sim->time_to_die;
	if (sim->times_to_eat == 0 && sim->times_to_eat != -2)
		return ;
	while (true)
	{
		if (philo_eat(seat, sim))
		{
			if (seat->times_eaten == sim->times_to_eat)
				break ;
			msg_add(sim->msg_queue, seat->id, "is sleeping");
			philo_spend_time(sim->time_to_sleep);
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
	while (seat->seat_taken == true)
		seat = seat->next;
	seat->seat_taken = true;
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
