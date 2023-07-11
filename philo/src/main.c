/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 18:30:00 by jbouma        ########   odam.nl         */
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
		return (errorlog("Wrong number of arguments"));
	sim = input(argc, argv, 0);
	if (!sim)
		return (EXIT_FAILURE);
	if (sim->philosophers > 200)
		return (free(sim), errorlog("Program accepts upto 200 philosophers"));
	sim->msg_queue = ft_calloc(1, sizeof(struct s_msg_queue));
	if (!sim->msg_queue)
		return (free(sim), errorlog("Malloc failed"));
	pthread_mutex_init(&sim->msg_queue->mutex, NULL);
	sim->start = timestamp();
	sim->table = table_prepare(sim->philosophers);
	if (!sim->table)
		return (free_willy(sim), errorlog("Malloc failed"));
	philo_join_table(sim);
	if (watch_them_die(sim))
		return (free_willy(sim), EXIT_SUCCESS);
	return (free_willy(sim), EXIT_FAILURE);
}
