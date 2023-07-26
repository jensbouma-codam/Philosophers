/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/14 18:50:24 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/26 15:07:59 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	ft_strtoi(const char *str)
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
	if (nbr == 0 && ALLOW_ZERO == 0)
		return (errorlog("Program does not accept 0 as agrument"), -1);
	return (nbr);
}

static t_sim	*input_argumenst(int argc, char **argv, int i)
{
	t_sim	*sim;
	int		n;

	sim = ft_calloc(1, sizeof(t_sim));
	if (!sim)
		return (errorlog("Malloc failed"), NULL);
	sim->times_to_eat = -1;
	while (++i < argc)
	{
		n = ft_strtoi(argv[i]);
		if (n < 0)
			return (free(sim), NULL);
		if (i == 1)
			sim->count = n;
		else if (i == 2)
			sim->n_to_die = (n * 1000);
		else if (i == 3)
			sim->n_to_eat = (n * 1000);
		else if (i == 4)
			sim->n_to_sleep = (n * 1000);
		else if (i == 5)
			sim->times_to_eat = n;
	}
	return (sim);
}

t_sim	*input(int argc, char **argv, int i)
{
	t_sim	*sim;

	if (argc < 5 || argc > 6)
		return (errorlog("Wrong number of arguments"), NULL);
	sim = input_argumenst(argc, argv, i);
	if (!sim)
		return (NULL);
	if (sim->count > PROCESS_LIMIT)
		return (free(sim), errorlog("Philosophers > PROCESS_LIMIT!"), NULL);
	return (sim);
}
