/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/25 02:58:50 by jensbouma     ########   odam.nl         */
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
	while (!sim->has_eaten.get(&sim->has_eaten) && id < sim->count)
	{
		philo = (t_philo *)sim->philos.get(&sim->philos, id);
		if (philo->t_eaten.get(&philo->t_eaten) < sim->times_to_eat)
			return (false);
		id++;
	}
	if (!sim->has_eaten.get(&sim->has_eaten))
	{
		msg_add(sim, -1, "Everbody has eaten!", true);
		sim->has_eaten.set(&sim->has_eaten, true);
	}
	return (true);
}

bool	someone_died(t_sim *sim)
{
	int			id;
	t_philo		*philo;

	id = 0;
	if (sim->someone_died.get(&sim->someone_died))
		return (true);
	while (id < sim->count)
	{
		philo = (t_philo *)sim->philos.get(&sim->philos, id);
		if (pthread_mutex_lock(&philo->t_to_die_mutex) != 0)
			return (errorlog("Failed to lock mutex"), false);
		if (philo->t_to_die < timestamp())
		{
			msg_add(sim, id, "Died!", true);
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

int	simulation(t_sim *s)
{
	int		id;

	id = 0;
	while (id < s->count)
		philo_create(s, id++);
	while (msg_print(s))
	{
		if (someone_died(s))
			break ;
		if (everbody_has_eaten(s))
			break ;
		usleep(100);
	}
	while (processcount(s) > 0)
		usleep(100);
	msg_print(s);
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
