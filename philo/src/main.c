/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/27 02:17:29 by jensbouma     ########   odam.nl         */
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

	if (DEBUG == 2)
		atexit(print_mem);
	s = input(argc, argv, 0);
	if (!s)
		return (EXIT_FAILURE);
	if (!v_init(&s->msg, sizeof(t_msg), msg_free, NULL)
		|| !v_init(&s->philos, sizeof(t_philo), philo_free, NULL))
		return (free(s), errorlog("Malloc failed"));
	if (!value_init(&s->end_sim)
		|| pthread_mutex_init(&s->msg_mutex, NULL) != 0
		|| pthread_mutex_init(&s->time_mutex, NULL) != 0)
		return (free(s), errorlog("Failed to init program"));
	if (!simulation(s))
		return (v_free(&s->philos), v_free(&s->msg), free(s), EXIT_FAILURE);
	if (pthread_mutex_destroy(&s->msg_mutex) != 0
		|| pthread_mutex_destroy(&s->time_mutex) != 0)
		return (errorlog("Failed to destroy mutex"), FAILURE);
	return (v_free(&s->philos), v_free(&s->msg), free(s), EXIT_SUCCESS);
}
