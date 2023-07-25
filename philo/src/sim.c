/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 18:04:15 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 19:10:01 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	everbody_has_eaten(t_sim *sim)
{
	int			id;
	t_philo		*philo;

	id = 0;
	if (sim->times_to_eat == -1)
		return (false);
	while (id < sim->count)
	{
		philo = (t_philo *)sim->philos.get(&sim->philos, id);
		if (philo->t_eaten.get(&philo->t_eaten) < sim->times_to_eat)
			return (false);
		id++;
	}
	if (!sim->has_eaten.get(&sim->has_eaten))
	{
		msg_add(sim, -1, "All philosophers have eaten enough", true);
		sim->has_eaten.set(&sim->has_eaten, true);
	}
	return (true);
}

bool	someone_died(t_sim *sim)
{
	int			id;
	t_philo		*philo;

	id = 0;
	while (id < sim->count)
	{
		philo = (t_philo *)sim->philos.get(&sim->philos, id);
		if (pthread_mutex_lock(&philo->t_to_die_mutex) != 0)
			return (errorlog("Failed to lock mutex"), false);
		if (philo->t_to_die < timestamp(sim)
			&& philo->running.get(&philo->running))
		{
			msg_add(sim, id, "died", true);
			sim->one_died.set(&sim->one_died, true);
			pthread_mutex_unlock(&philo->t_to_die_mutex);
			return (true);
		}
		pthread_mutex_unlock(&philo->t_to_die_mutex);
		id++;
	}
	return (false);
}

int	processcount(t_sim *sim)
{
	int		id;
	int		count;

	id = 0;
	count = 0;
	while (id < sim->count)
	{
		if (((t_philo *)sim->philos.get(&sim->philos, id))->running
			.get(&((t_philo *)sim->philos.get(&sim->philos, id))->running))
			count++;
		id++;
	}
	return (count);
}

void	simulation_monitor(t_sim *s)
{
	while (msg_print(s))
	{
		if (someone_died(s))
			break ;
		if (everbody_has_eaten(s))
			break ;
		spend_time(s, 2000);
	}
	msg_print(s);
	while (processcount(s) > 0)
		sleep(1);
}

int	simulation(t_sim *s)
{
	int				id;
	t_philo			*p;

	id = 0;
	v_init(&s->forks, sizeof(t_fork), fork_free, NULL);
	fork_create(s);
	while (id < s->count)
		philo_create(s, id++);
	pthread_mutex_lock(&s->start_time_mutex);
	s->start_lock = true;
	pthread_mutex_unlock(&s->start_time_mutex);
	simulation_monitor(s);
	while (--id >= 0)
	{
		p = (t_philo *)s->philos.get(&s->philos, id);
		p->running.free(&p->running);
		p->t_eaten.free(&p->t_eaten);
		id--;
	}
	s->has_eaten.free(&s->has_eaten);
	s->one_died.free(&s->one_died);
	v_free(&s->forks);
	if (pthread_mutex_destroy(&s->msg_mutex) != 0)
		return (errorlog("Failed to destroy mutex"), FAILURE);
	return (SUCCESS);
}
