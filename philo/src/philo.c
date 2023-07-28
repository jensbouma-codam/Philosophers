/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:34:38 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/28 15:24:45 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_free(void *ptr)
{
	t_philo	*p;

	p = (t_philo *)ptr;
	while (p->run.get(&p->run) == RUNNING || p->run.get(&p->run) == CREATE)
	{
		if (p->run.get(&p->run) == FAIL)
			break ;
		usleep(100);
	}
	pthread_detach(p->thread);
	value_free(&p->dead_time);
	value_free(&p->eaten);
	value_free(&p->run);
	free(p);
	return (SUCCESS);
}

void	philo_eat(t_philo *p, t_sim *s, t_fork *fork_l, t_fork *fork_r)
{
	pthread_mutex_lock(&fork_l->in_use_mutex);
	msg_add(s, p->id, "has taken a fork", false);
	pthread_mutex_lock(&fork_r->in_use_mutex);
	msg_add(s, p->id, "has taken a fork", false);
	p->dead_time.set(&p->dead_time, timestamp(s) + s->time_to_die);
	msg_add(s, p->id, "is eating", false);
	spend_time(s, s->time_to_eat);
	p->eaten.set(&p->eaten, p->eaten.get(&p->eaten) + 1);
	pthread_mutex_unlock(&fork_r->in_use_mutex);
	pthread_mutex_unlock(&fork_l->in_use_mutex);
}

void	wait_for_start(t_sim *s, t_philo *p)
{
	while (true)
	{
		if (s->end_sim.get(&s->end_sim))
			return ;
		else if (s->start_lock.get(&s->start_lock) == false)
			break ;
		usleep(100);
	}
	if (p->id % 2 == 1)
	{
		msg_add(s, p->id, "is thinking", false);
		spend_time(s, s->time_to_eat / 2);
	}
}

void	*philo_proc(void *ptr)
{
	t_philo	*p;
	t_sim	*s;
	t_fork	*fork_r;
	t_fork	*fork_l;

	p = (t_philo *)ptr;
	s = p->sim;
	if (p->id + 1 == s->count)
		fork_r = (t_fork *)s->forks.get(&s->forks, 0);
	else
		fork_r = (t_fork *)s->forks.get(&s->forks, (p->id + 1));
	fork_l = (t_fork *)s->forks.get(&s->forks, p->id);
	p->run.set(&p->run, RUNNING);
	wait_for_start(s, p);
	while (!s->end_sim.get(&s->end_sim))
	{
		if (fork_l->id == fork_r->id)
			continue ;
		philo_eat(p, s, fork_l, fork_r);
		msg_add(s, p->id, "is sleeping", false);
		spend_time(s, s->time_to_sleep);
		msg_add(s, p->id, "is thinking", false);
	}
	p->run.set(&p->run, STOPED);
	return (NULL);
}

int	philo_create(t_sim *s, int id)
{
	t_philo		*p;

	p = ft_calloc(1, sizeof(t_philo));
	if (p)
	{
		p->id = id;
		p->sim = s;
		if (value_init(&p->run)
			&& value_init(&p->eaten)
			&& value_init(&p->dead_time)
			&& s->philos.add(&s->philos, (void *)p))
		{
			p->dead_time.set(&p->dead_time, s->time_to_die);
			p->run.set(&p->run, CREATE);
			if (pthread_create(&p->thread, NULL, philo_proc, p) != 0)
				return (errorlog("Failed to create thread"), FAILURE);
			return (SUCCESS);
		}
		free(p);
	}
	value_free(&p->run);
	value_free(&p->eaten);
	value_free(&p->dead_time);
	return (errorlog("Failed to create philo"), FAILURE);
}
