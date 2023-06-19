/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/20 00:42:57 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_mem(void)
{
	system("leaks philo");
}

void	wakeup_machine(void)
{
	uint64_t	i;

	i = 0;
	while (i < __INT64_MAX__)
		i++;
}

int	main(int argc, char **argv)
{
	struct s_arg	*a;
	struct s_table	*table;

	if (DEBUG == 2)
		atexit(print_mem);
	wakeup_machine();
	a = input(argc, argv);
	if (a->philosophers > 3000)
		error_exit("Program accepts upto 100 philosophers");
	table = table_cutlery(a->philosophers);
	table_mutex_init(table);
	table_join(table, a);
	table_tread_create(table);
	return (watch_them_die(table));
	return (EXIT_FAILURE);
}
