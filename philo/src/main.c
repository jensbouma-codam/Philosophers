/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/21 04:40:29 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	struct s_simulation		*sim;

	if (DEBUG == 2)
		atexit(print_mem);
	sim = input(argc, argv);
	if (sim->philosophers > 4000)
		error_exit("Program accepts upto 4000 philosophers");
	sim->table = table_cutlery(sim->philosophers);
	table_join(sim);
	table_mutex_init(sim);
	sim->printer = mem_add(1, sizeof(struct s_msg));
	sim->printer->msg = NULL;
	pthread_mutex_init(&sim->printer->mutex, NULL);
	pthread_mutex_init(&sim->mutex, NULL);
	table_tread_create(sim);
	return (watch_them_die(sim));
}
