/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 18:04:15 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/27 01:53:01 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	everbody_has_eaten(t_sim *s)
{
	int			id;
	t_philo		*philo;

	id = 0;
	if (s->times_to_eat == -1)
		return (false);
	while (id < s->count)
	{
		philo = (t_philo *)s->philos.get(&s->philos, id);
		if (philo->x_eaten.get(&philo->x_eaten) < s->times_to_eat)
			return (false);
		id++;
	}
	if (DEBUG)
		msg_add(s, -1, "All philosophers have eaten enough", true);
	return (true);
}

bool	someone_died(t_sim *s)
{
	int			id;
	t_philo		*philo;

	id = 0;
	while (id < s->count)
	{
		philo = (t_philo *)s->philos.get(&s->philos, id);
		pthread_mutex_lock(&philo->time_to_die_mutex);
		if (philo->time_to_die < timestamp(s)
			&& philo->running.get(&philo->running))
		{
			msg_add(s, id, "died", true);
			pthread_mutex_unlock(&philo->time_to_die_mutex);
			return (true);
		}
		pthread_mutex_unlock(&philo->time_to_die_mutex);
		id++;
	}
	return (false);
}

int	processcount(t_sim *s)
{
	int		id;
	int		count;

	id = 0;
	count = 0;
	while (id < s->count)
	{
		if (((t_philo *)s->philos.get(&s->philos, id))->running
			.get(&((t_philo *)s->philos.get(&s->philos, id))->running))
			count++;
		id++;
	}
	return (count);
}

void	simulation_run(t_sim *s)
{
	int	count;

	count = 0;
	timestamp(s);
	s->start_lock.set(&s->start_lock, false);
	while (true)
	{
		if (someone_died(s) || everbody_has_eaten(s))
			break ;
		else if (count++ == 100)
		{
			msg_print(s);
			count = 0;
		}
		else
			usleep(100);
	}
	s->end_sim.set(&s->end_sim, true);
	msg_print(s);
	while (processcount(s) > 0)
		usleep(1000);
}

int	simulation(t_sim *s)
{
	int				id;
	t_philo			*p;

	id = 0;
	v_init(&s->forks, sizeof(t_fork), fork_free, NULL);
	value_init(&s->start_lock);
	s->start_lock.set(&s->start_lock, true);
	if (!fork_create(s))
		return (s->forks.free(&s->forks), FAILURE);
	while (id < s->count)
		if (!philo_create(s, id++))
			return (s->forks.free(&s->forks), FAILURE);
	simulation_run(s);
	while (id--)
	{
		p = (t_philo *)s->philos.get(&s->philos, id);
		p->running.free(&p->running);
		p->x_eaten.free(&p->x_eaten);
	}
	s->end_sim.free(&s->end_sim);
	v_free(&s->forks);
	if (pthread_mutex_destroy(&s->msg_mutex) != 0)
		return (errorlog("Failed to destroy mutex"), FAILURE);
	return (SUCCESS);
}
