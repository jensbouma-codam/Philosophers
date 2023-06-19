/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 18:50:24 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/19 21:23:28 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static uint32_t	mystrtoull(const char *str)
{
	int			i;
	uint64_t	nbr;

	i = 0;
	nbr = 0;
	if ((str[i] == '-'))
		error_exit("Program only accepts positive numbers");
	while (str[i])
	{
		if (str[i] == '+')
			i++;
		if (!isdigit(str[i]))
			error_exit("Program only accepts numbers");
		if ((nbr * 10) + (str[i] - '0') > INT32_MAX)
			error_exit("Program accepts numbers upto 2147483647");
		nbr = (nbr * 10) + (str[i] - '0');
		i++;
	}
	return (nbr);
}

struct s_arg	input(int argc, char **argv)
{
	int				i;
	uint32_t		n;
	struct s_arg	a;

	if (argc < 5 || argc > 6)
		error_exit("Wrong number of arguments");
	i = 1;
	a.must_eat = -1;
	while (i < argc)
	{
		n = mystrtoull(argv[i]);
		if (i == 1)
			a.philosophers = n;
		else if (i == 2)
			a.time_to_die = (n * 1000);
		else if (i == 3)
			a.time_to_eat = (n * 1000);
		else if (i == 4)
			a.time_to_sleep = (n * 1000);
		else if (i == 5)
			a.must_eat = n;
		i++;
	}
	return (a);
}
