/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 18:04:15 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/28 15:19:55 by jensbouma     ########   odam.nl         */
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
		if (philo->eaten.get(&philo->eaten) < s->times_to_eat)
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
		if (philo->dead_time.get(&philo->dead_time) < timestamp(s)
			&& philo->run.get(&philo->run))
		{
			msg_add(s, id, "died", true);
			return (true);
		}
		id++;
	}
	return (false);
}

void	simulation_run(t_sim *s)
{
	int	wait;

	wait = 0;
	timestamp(s);
	msg_print(s);
	s->start_lock.set(&s->start_lock, false);
	while (true)
	{
		if (someone_died(s) || everbody_has_eaten(s))
			break ;
		else if (wait++ >= 1)
		{
			msg_print(s);
			wait = 0;
		}
		else
			usleep(1000);
	}
	s->end_sim.set(&s->end_sim, true);
	msg_print(s);
}

int	simulation(t_sim *s)
{
	int				id;

	id = 0;
	if (!value_init(&s->start_lock) || !value_init(&s->end_sim))
		return (errorlog("Simulation init failed"), FAILURE);
	else
	{
		s->start_lock.set(&s->start_lock, true);
		while (id < s->count)
		{
			if (!philo_create(s, id++))
			{
				s->end_sim.set(&s->end_sim, true);
				return (FAILURE);
			}
		}
	}
	simulation_run(s);
	return (SUCCESS);
}
