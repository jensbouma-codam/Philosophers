/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:34:38 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 02:59:17 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_free(void *ptr)
{
	t_philo	*p;

	p = (t_philo *)ptr;
	if (pthread_detach(p->thread) != 0)
		errorlog("Failed to detach monitor thread");
	free(p);
	return (SUCCESS);
}

void	*philo_proc(void *ptr)
{
	t_philo	*p;
	t_sim	*s;

	p = (t_philo *)ptr;
	s = p->sim;
	p->running.set(&p->running, true);
	while (!s->has_eaten.get(&s->has_eaten)
		&& !s->someone_died.get(&s->someone_died))
	{
		msg_add(s, p->id, "has taken a fork", false);
		msg_add(s, p->id, "has taken a fork", false);
		msg_add(s, p->id, "is eating", false);
		spend_time(s, s->t_to_eat);
		p->t_eaten.set(&p->t_eaten, p->t_eaten.get(&p->t_eaten) + 1);
		msg_add(s, p->id, "is sleeping", false);
		spend_time(s, s->t_to_sleep);
		msg_add(s, p->id, "is thinking", false);
	}
	p->running.set(&p->running, false);
	return (NULL);
}

int	philo_create(t_sim *s, int id)
{
	t_philo		*p;

	p = ft_calloc(1, sizeof(t_philo));
	if (!p)
		return (errorlog("Malloc failed"), FAILURE);
	p->id = id;
	value_init(&p->t_eaten);
	value_init(&p->running);
	p->sim = s;
	if (pthread_mutex_init(&p->t_to_die_mutex, NULL) != 0)
		return (errorlog("Failed to init mutex"), FAILURE);
	if (id + 1 == s->count)
		printf("%i This one need the first fork on the right!\n", id);
	if (s->philos.add(&s->philos, (void *)p) == FAILURE)
		return (free(p), errorlog("Malloc failed"), FAILURE);
	p->t_to_die = timestamp() + s->t_to_die;
	if (pthread_create(&p->thread, NULL, philo_proc, p) != 0)
		return (errorlog("Failed to create monitor thread"), FAILURE);
	return (SUCCESS);
}
