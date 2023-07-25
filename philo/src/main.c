/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/25 14:52:06 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

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
		// msg_add(sim, -1, "Everbody has eaten!", true);
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
		if (philo->t_to_die < timestamp(false) && philo->running.get(&philo->running))
		{
			msg_add(sim, id, "died", true);
			sim->someone_died.set(&sim->someone_died, true);
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
int fork_free(void *ptr)
{
	t_fork	*fork;

	fork = (t_fork *)ptr;
	fork->in_use.free(&fork->in_use);
	free(fork);
	return (SUCCESS);
}

int fork_create(t_sim *s)
{
	int			id;
	t_fork		*fork;

	id = 0;
	while (id < s->count)
	{
		fork = ft_calloc(1, sizeof(t_fork));
		if (!fork)
			return (errorlog("Malloc failed"), FAILURE);
		value_init(&fork->in_use);
		fork->id = id;
		pthread_mutex_init(&fork->in_use_mutex, NULL);
		if (!s->forks.add(&s->forks, fork))
			return (errorlog("Failed to add fork"), FAILURE);
		id++;
	}
	return (SUCCESS);
}

int	simulation(t_sim *s)
{
	int		id;
	t_philo	*p;

	id = 0;
	v_init(&s->forks, sizeof(t_fork), fork_free, NULL);
	fork_create(s);
	while (id < s->count)
	{
		philo_create(s, id++);
		spend_time(s, 10);
		// if (someone_died(s))
		// 	break ;
	}
	timestamp(true);
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
	while (--id >= 0)
	{
		p = (t_philo *)s->philos.get(&s->philos, id);
		p->running.free(&p->running);
		p->t_eaten.free(&p->t_eaten);
		id--;
	}
	s->has_eaten.free(&s->has_eaten);
	s->someone_died.free(&s->someone_died);
	v_free(&s->forks);
	if (pthread_mutex_destroy(&s->msg_mutex) != 0)
		return (errorlog("Failed to destroy mutex"), FAILURE);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_sim	*s;

	if (DEBUG == 2)
		atexit(print_mem);
	s = input(argc, argv, 0);
	if (!s)
		return (EXIT_FAILURE);
	if (!v_init(&s->msg, sizeof(t_msg), msg_free, NULL)
		|| !v_init(&s->philos, sizeof(t_philo), philo_free, NULL))
		return (free(s), errorlog("Malloc failed"));
	value_init(&s->has_eaten);
	value_init(&s->someone_died);
	s->msg_lock = false;
	pthread_mutex_init(&s->msg_mutex, NULL);
	if (!simulation(s))
		return (v_free(&s->philos), v_free(&s->msg), free(s), EXIT_FAILURE);
	return (v_free(&s->philos), v_free(&s->msg), free(s), EXIT_SUCCESS);
}
