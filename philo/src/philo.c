/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:34:38 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/26 19:10:45 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_free(void *ptr)
{
	t_philo	*p;

	p = (t_philo *)ptr;
	if (pthread_detach(p->thread) != 0)
		errorlog("Failed to detach thread");
	free(p);
	return (SUCCESS);
}

void	philo_think(t_philo *p, t_sim *s)
{
	if (p->state != THINKING)
		msg_add(s, p->id, "is thinking", false);
	p->state = THINKING;
}

void	philo_eat(t_philo *p, t_sim *s, t_fork *fork_l, t_fork *fork_r)
{
	if (p->id % 2 == 0)
	{
		usleep(1000);
		pthread_mutex_lock(&fork_r->in_use_mutex);
		pthread_mutex_lock(&fork_l->in_use_mutex);
	}
	else
	{
		pthread_mutex_lock(&fork_l->in_use_mutex);
		pthread_mutex_lock(&fork_r->in_use_mutex);
	}
	msg_add(s, p->id, "has taken a fork", false);
	msg_add(s, p->id, "has taken a fork", false);
	p->state = EATING;
	msg_add(s, p->id, "is eating", false);
	pthread_mutex_lock(&p->n_to_die_mutex);
	p->n_to_die = timestamp(s) + s->n_to_die;
	pthread_mutex_unlock(&p->n_to_die_mutex);
	spend_time(s, s->n_to_eat);
	p->x_eaten.set(&p->x_eaten, p->x_eaten.get(&p->x_eaten) + 1);
	msg_add(s, p->id, "is sleeping", false);
	pthread_mutex_unlock(&fork_r->in_use_mutex);
	pthread_mutex_unlock(&fork_l->in_use_mutex);
}

void	*philo_proc(void *ptr)
{
	t_philo	*p;
	t_sim	*s;
	t_fork	*fork_r;

	p = (t_philo *)ptr;
	s = p->sim;
	p->state = INIT;
	if (p->id + 1 == s->count)
		fork_r = (t_fork *)s->forks.get(&s->forks, 0);
	else
		fork_r = (t_fork *)s->forks.get(&s->forks, (p->id + 1));
	p->running.set(&p->running, true);
	pthread_mutex_lock(&s->start_lock_mutex);
	pthread_mutex_unlock(&s->start_lock_mutex);
	while (!s->has_eaten.get(&s->has_eaten) && !s->one_died.get(&s->one_died))
	{
		if (((t_fork *)s->forks.get(&s->forks, p->id))->id == fork_r->id)
			continue ;
		philo_think(p, s);
		philo_eat(p, s, (t_fork *)s->forks.get(&s->forks, p->id), fork_r);
		spend_time(s, s->n_to_sleep);
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
	if (pthread_mutex_init(&p->n_to_die_mutex, NULL) != 0)
		return (errorlog("Failed to init mutex"), FAILURE);
	if (s->philos.add(&s->philos, (void *)p) == FAILURE)
		return (free(p), errorlog("Malloc failed"), FAILURE);
	p->id = id;
	p->n_to_die = s->n_to_die;
	p->sim = s;
	if (!value_init(&p->x_eaten) || !value_init(&p->running))
		return (errorlog("Failed to init value"), FAILURE);
	if (pthread_create(&p->thread, NULL, philo_proc, p) != 0)
		return (errorlog("Failed to create thread"), FAILURE);
	return (SUCCESS);
}
