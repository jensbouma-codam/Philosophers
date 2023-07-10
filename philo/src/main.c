/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/10 22:19:27 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	struct s_simulation	*sim;

	if (DEBUG == 2)
		atexit(print_mem);
	sim = input(argc, argv);
	if (sim->philosophers > MAX_THREATS)
		error_exit("Program accepts upto 709 philosophers");
	if (sim->philosophers == 0 || sim->time_to_die == 0)
		error_exit("Program doesn't accept zero's for amount and time to die");
	sim->msg_queue = mem_add(1, sizeof(struct s_msg_queue));
	pthread_mutex_init(&sim->msg_queue->mutex, NULL);
	pthread_mutex_init(&sim->mutex, NULL);
	sim->table = table_add_cutlery(sim->philosophers);
	table_add_seats(sim);
	table_add_mutexes(sim);
	philo_join_table(sim);
	return (watch_them_die(sim));
}
