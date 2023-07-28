/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   forks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 18:02:16 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/28 14:57:11 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_free(void *ptr)
{
	t_fork	*fork;

	fork = (t_fork *)ptr;
	if (pthread_mutex_destroy(&fork->in_use_mutex) != 0)
		errorlog("Failed to destroy in_use_mutex mutex");
	free(ptr);
	return (SUCCESS);
}

int	fork_create(t_sim *s)
{
	int			id;
	t_fork		*fork;

	id = 0;
	while (id < s->count)
	{
		fork = ft_calloc(1, sizeof(t_fork));
		if (!fork)
			return (errorlog("Malloc failed"), FAILURE);
		fork->id = id;
		if (pthread_mutex_init(&fork->in_use_mutex, NULL) != 0)
			return (free(fork), errorlog("Failed to init mutex"), FAILURE);
		if (!s->forks.add(&s->forks, fork))
			return (free(fork), errorlog("Failed to add fork"), FAILURE);
		id++;
	}
	return (SUCCESS);
}
