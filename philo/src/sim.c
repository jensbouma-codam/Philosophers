/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 18:04:15 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/28 10:56:30 by jensbouma     ########   odam.nl         */
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
			&& philo->running.get(&philo->running))
		{
			msg_add(s, id, "died", true);
			return (true);
		}
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
	if (!v_init(&s->forks, sizeof(t_fork), fork_free, NULL)
		|| !value_init(&s->start_lock))
		return (errorlog("Malloc failed"), FAILURE);
	s->start_lock.set(&s->start_lock, true);
	if (!fork_create(s))
		return (v_free(&s->forks), FAILURE);
	while (id < s->count)
		if (!philo_create(s, id++))
			return (v_free(&s->forks), FAILURE);
	simulation_run(s);
	while (id--)
	{
		p = (t_philo *)s->philos.get(&s->philos, id);
		p->running.free(&p->running);
		p->eaten.free(&p->eaten);
	}
	s->end_sim.free(&s->end_sim);
	v_free(&s->forks);
	return (SUCCESS);
}
