/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 18:50:24 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/14 21:29:41 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	strtoi(const char *str)
{
	int	i;
	int	neg;
	int	nbr;

	i = 0;
	neg = 1;
	nbr = 0;
	if (str[i] == '-')
		neg = -1;
	if ((str[i] == '-') || (str[i] == '+'))
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			error_exit("Not a number");
		nbr = (nbr * 10) + (str[i] - '0');
		i++;
	}
	nbr = nbr * neg;
	return (nbr);
}

struct s_arg	input(int argc, char **argv)
{
	int				i;
	u_int32_t		n;
	struct s_arg	a;

	if (argc < 5 || argc > 6)
		error_exit("Wrong number of arguments");
	i = 1;
	while (i < argc)
	{
		n = strtoi(argv[i]);
		if (n < 0)
			error_exit("Negative number");
		if (i == 1)
			a.philosophers = n;
		else if (i == 2)
			a.time_to_die = n * 1000;
		else if (i == 3)
			a.time_to_eat = n * 1000;
		else if (i == 4)
			a.time_to_sleep = n * 1000;
		else if (i == 5)
			a.must_eat = n;
		i++;
	}
	return (a);
}
