/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 18:50:24 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 18:24:01 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int32_t	mystrtoull(const char *str)
{
	int			i;
	uint64_t	nbr;

	i = 0;
	nbr = 0;
	if ((str[i] == '-'))
		return (errorlog("Program only accepts positive numbers"), -1);
	while (str[i])
	{
		if (str[i] == '+')
			++i;
		if (!isdigit(str[i]))
			return (errorlog("Program only accepts numbers"), -1);
		if ((nbr * 10) + (str[i] - '0') > INT32_MAX)
			return (errorlog("Program accepts numbers upto 2147483647"), -1);
		nbr = (nbr * 10) + (str[i] - '0');
		++i;
	}
	return (nbr);
}

/**
 * @brief Get 4 or 5 input arguments from the command line
 * and process them into a struct s_simulation.
 * 
 * The first argument is the number of philosophers.
 * The second argument is the time to die in milliseconds.
 * The third argument is the time to eat in milliseconds.
 * The fourth argument is the time to sleep in milliseconds.
 * The fifth argument is the number of times each philosopher
 * must eat before the simulation stops.
 * 
 * @param argc 
 * @param argv 
 * @return struct s_simulation* 
 */
struct s_simulation	*input(int argc, char **argv, int i)
{
	int32_t				n;
	struct s_simulation	*sim;

	sim = ft_calloc(1, sizeof(struct s_simulation));
	if (!sim)
		return (errorlog("Malloc failed"), NULL);
	sim->times_to_eat = -1;
	while (++i < argc)
	{
		n = mystrtoull(argv[i]);
		if (n <= 0)
			return (free(sim), errorlog("Program doesn't accept zero's"), NULL);
		if (i == 1)
			sim->philosophers = n;
		else if (i == 2)
			sim->time_to_die = (n * 1000);
		else if (i == 3)
			sim->time_to_eat = (n * 1000);
		else if (i == 4)
			sim->time_to_sleep = (n * 1000);
		else if (i == 5)
			sim->times_to_eat = n;
	}
	return (sim);
}
