/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:34:38 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 15:26:49 by jensbouma     ########   odam.nl         */
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
	t_fork	*fork_l;
	t_fork	*fork_r;

	p = (t_philo *)ptr;
	s = p->sim;
	p->state = INIT;
	fork_l = (t_fork *)s->forks.get(&s->forks, p->id);
	if (p->id + 1 == s->count)
		fork_r = (t_fork *)s->forks.get(&s->forks, 0);
	else
		fork_r = (t_fork *)s->forks.get(&s->forks, (p->id + 1));
	p->running.set(&p->running, true);
	while (!s->has_eaten.get(&s->has_eaten) && !s->someone_died.get(&s->someone_died))
	{
		if (timestamp(false) == -1 || fork_l->id == fork_r->id)
			continue;
		// {
		// 	usleep(1000);
		// }
		// if (timestamp(false) == 0)
		// {
		// 	if (p->id % 2 == 0)
		// 		usleep(1);
		// }
		if (p->id % 2 == 0 && !s->someone_died.get(&s->someone_died))
		{
			spend_time(s, 1000);
			pthread_mutex_lock(&fork_r->in_use_mutex);
			pthread_mutex_lock(&fork_l->in_use_mutex);
		}
		else if (!s->someone_died.get(&s->someone_died))
		{
			pthread_mutex_lock(&fork_l->in_use_mutex);
			pthread_mutex_lock(&fork_r->in_use_mutex);
		}
		msg_add(s, p->id, "has taken a fork", false);
		msg_add(s, p->id, "has taken a fork", false);
		msg_add(s, p->id, "is eating", false);
		p->state = EATING;
		pthread_mutex_lock(&p->t_to_die_mutex);
		p->t_to_die = timestamp(false) + s->t_to_die + 1000;
		pthread_mutex_unlock(&p->t_to_die_mutex);
		p->t_eaten.set(&p->t_eaten, p->t_eaten.get(&p->t_eaten) + 1);
		spend_time(s, s->t_to_eat);
		pthread_mutex_unlock(&fork_r->in_use_mutex);
		pthread_mutex_unlock(&fork_l->in_use_mutex);
		msg_add(s, p->id, "is sleeping", false);
		spend_time(s, s->t_to_sleep);
		// if (fork_l->in_use.get(&fork_r->in_use) == false && fork_l->id != fork_r->id)
		// {
		// 	fork_l->in_use.set(&fork_r->in_use, true);
		// 	if (fork_r->in_use.get(&fork_l->in_use) == false)
		// 	{
		// 		msg_add(s, p->id, "has taken a fork", false);
		// 		fork_r->in_use.set(&fork_l->in_use, true);	
		// 		msg_add(s, p->id, "has taken a fork", false);
		// 		msg_add(s, p->id, "is eating", false);
		// 		p->state = EATING;
		// 		pthread_mutex_lock(&p->t_to_die_mutex);
		// 		p->t_to_die = timestamp(false) + s->t_to_die;
		// 		pthread_mutex_unlock(&p->t_to_die_mutex);
		// 		p->t_eaten.set(&p->t_eaten, p->t_eaten.get(&p->t_eaten) + 1);
		// 		spend_time(s, s->t_to_eat);
		// 		fork_r->in_use.set(&fork_r->in_use, false);
		// 		fork_l->in_use.set(&fork_l->in_use, false);
		// 		msg_add(s, p->id, "is sleeping", false);
		// 		spend_time(s, s->t_to_sleep);
		// 	}
		// 	else
		// 		fork_l->in_use.set(&fork_l->in_use, false);
		// }
		// spend_time(s, (s->t_to_die - timestamp(false)) / 10);
		if (p->state != THINKING)
		{
			msg_add(s, p->id, "is thinking", false);
			p->state = THINKING;
			// if (p->id % 2 == 0)
			// 	spend_time(s, 2000);
			// else
			// 	spend_time(s, 1000);
		}
		// spend_time(s, s->count * 10);
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
	if (pthread_mutex_init(&p->t_to_die_mutex, NULL) != 0)
		return (errorlog("Failed to init mutex"), FAILURE);
	if (s->philos.add(&s->philos, (void *)p) == FAILURE)
		return (free(p), errorlog("Malloc failed"), FAILURE);
	if (p->id % 2 == 0)
		usleep(1);
	p->id = id;
	p->t_to_die = s->t_to_die;
	p->sim = s;
	value_init(&p->t_eaten);
	value_init(&p->running);
	if (pthread_create(&p->thread, NULL, philo_proc, p) != 0)
		return (errorlog("Failed to create monitor thread"), FAILURE);
	return (SUCCESS);
}
