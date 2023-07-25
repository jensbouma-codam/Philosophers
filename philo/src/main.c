/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/25 02:33:06 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

bool	everbody_has_eaten(t_sim *sim)
{
	static bool	everbody = false;
	int			id;
	t_philo		*philo;

	id = 0;
	if (sim->times_to_eat == -1)
		return (false);
	while (!everbody && id < sim->count)
	{
		philo = (t_philo *)sim->philos.get(&sim->philos, id);
		if (philo->t_eaten.get(&philo->t_eaten) < sim->times_to_eat)
			return (false);
		id++;
	}
	if (!everbody)
		msg_add(sim, -1, "Everbody has eaten!", true);
	everbody = true;
	return (everbody);
}

bool	someone_died(t_sim *sim)
{
	static bool	someone_died = false;
	int			id;
	t_philo		*philo;

	id = 0;
	if (someone_died)
		return (true);
	while (id < sim->count)
	{
		philo = (t_philo *)sim->philos.get(&sim->philos, id);
		if (pthread_mutex_lock(&philo->time_to_die_mutex) != 0)
			return (errorlog("Failed to lock mutex"), false);
		if (philo->time_to_die < timestamp())
		{
			msg_add(sim, id, "Died!", true);
			someone_died = true;
			pthread_mutex_unlock(&philo->time_to_die_mutex);
			return (true);
		}
		pthread_mutex_unlock(&philo->time_to_die_mutex);
		id++;
	}
	return (false);
}

int	get_processcount(t_sim *sim)
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
		{
			s->someone_died.set(&s->someone_died, true);
			break ;
		}
		if (everbody_has_eaten(s))
		{
			s->everbody_has_eaten.set(&s->everbody_has_eaten, true);
			break ;
		}
		usleep(100);
	}
	while (get_processcount(s) > 0)
	{
		printf("Ending %i processes\n", get_processcount(s));
		usleep(100000);
	}
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
	value_init(&s->proc);
	value_init(&s->everbody_has_eaten);
	value_init(&s->someone_died);
	s->msg_lock = false;
	pthread_mutex_init(&s->msg_mutex, NULL);
	pthread_mutex_init(&s->mutex, NULL);
	if (!simulation(s))
		return (v_free(&s->philos), v_free(&s->msg), free(s), EXIT_FAILURE);
	return (v_free(&s->philos), v_free(&s->msg), free(s), EXIT_SUCCESS);
}
