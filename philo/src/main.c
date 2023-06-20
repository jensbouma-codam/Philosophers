/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/20 04:17:15 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	struct s_arg	*a;
	struct s_table	*table;

	if (DEBUG == 2)
		atexit(print_mem);
	a = input(argc, argv);
	if (a->philosophers > 4000)
		error_exit("Program accepts upto 4000 philosophers");
	table = table_cutlery(a->philosophers);
	table_join(table, a);
	table_mutex_init(table, a);
	table_tread_create(table);
	return (watch_them_die(table));
}
