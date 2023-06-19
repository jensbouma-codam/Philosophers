/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:32 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/19 22:24:27 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	struct s_arg	a;
	struct s_table	*table;

	a = input(argc, argv);
	if (a.philosophers > 3000)
		error_exit("Program accepts upto 100 philosophers");
	table = table_cutlery(a.philosophers);
	table_mutex_init(table);
	table_join(table, a);
	table_tread_create(table);
	return (watch_them_die(table));
}
