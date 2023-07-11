/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 15:57:33 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

void	free_willy(struct s_simulation *sim)
{
	msg_print(sim->msg_queue);
	free(sim->msg_queue);
	while (sim->table)
	{
		free(sim->table->l_fork);
		free(sim->table);
		sim->table = sim->table->next;
	}
	free(sim);

}

int	main(int argc, char **argv)
{
	struct s_simulation	*sim;

	if (DEBUG == 2)
		atexit(print_mem);
	if (argc < 5 || argc > 6)
		return (error_exit("Wrong number of arguments"));
	sim = input(argc, argv);
	if (sim->philosophers > MAX_THREATS)
		return (free(sim), error_exit("Program accepts upto 709 philosophers"));
	sim->start = timestamp();
	if (sim->philosophers == 0 || sim->time_to_die == 0)
		return (free(sim),
			error_exit("Program doesn't accept zero's"));
	sim->msg_queue = mem_add(1, sizeof(struct s_msg_queue));
	if (!sim->msg_queue)
		return (free_willy(sim), EXIT_FAILURE);
	pthread_mutex_init(&sim->msg_queue->mutex, NULL);
	sim->table = table_add_cutlery(sim->philosophers);
	table_add_seats(sim);
	table_add_mutexes(sim);
	philo_join_table(sim);
	if (watch_them_die(sim))
		return (free_willy(sim), EXIT_SUCCESS);
	return (free_willy(sim), EXIT_FAILURE);
}
