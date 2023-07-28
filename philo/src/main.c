/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/28 15:21:15 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_mem(void)
{
	if (DEBUG == 2)
		system("leaks philo");
}

int	main(int argc, char **argv)
{
	t_sim	*s;
	int		exit;

	if (DEBUG == 2)
		atexit(print_mem);
	exit = EXIT_FAILURE;
	s = input(argc, argv, 0);
	if (!s)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&s->msg_mutex, NULL) != 0
		|| pthread_mutex_init(&s->time_mutex, NULL) != 0
		|| !v_init(&s->philos, sizeof(t_philo), philo_free, NULL)
		|| !v_init(&s->msg, sizeof(t_msg), msg_free, NULL)
		|| !v_init(&s->forks, sizeof(t_fork), fork_free, NULL)
		|| !fork_create(s))
		errorlog("Failed to init program");
	else
		exit = simulation(s);
	v_free(&s->philos);
	v_free(&s->forks);
	v_free(&s->msg);
	pthread_mutex_destroy(&s->time_mutex);
	pthread_mutex_destroy(&s->msg_mutex);
	free(s);
	return (exit);
}
