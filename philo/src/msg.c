/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   msg.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/20 22:35:49 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/26 15:54:14 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	printmsg(struct s_msg *msg)
{
	if (DEBUG > 0)
		ft_putint(msg->timestamp);
	else
		ft_putint(msg->timestamp);
	write(1, " ", 1);
	if (msg->id != -1)
	{
		ft_putint(msg->id);
		write(1, " ", 1);
	}
	write(1, msg->msg, ft_strlen(msg->msg));
	write(1, "\n", 1);
}

int	msg_print(t_sim *sim)
{
	t_msg	*msg;
	int		i;

	i = 0;
	pthread_mutex_lock(&sim->msg_mutex);
	while (i < sim->msg.size(&sim->msg))
	{
		msg = (t_msg *)sim->msg.get(&sim->msg, i);
		printmsg(msg);
		sim->msg.del(&sim->msg, i);
	}
	pthread_mutex_unlock(&sim->msg_mutex);
	return (SUCCESS);
}

int	msg_free(void *ptr)
{
	t_msg	*msg;

	msg = (t_msg *)ptr;
	free(msg);
	return (SUCCESS);
}

int	msg_add(t_sim *sim, int id, char *msg, bool last)
{
	t_msg		*new;
	static bool	lock = false;

	pthread_mutex_lock(&sim->msg_mutex);
	if (lock && id != -1)
		return (pthread_mutex_unlock(&sim->msg_mutex), SUCCESS);
	new = ft_calloc(1, sizeof(t_msg));
	if (!new)
	{
		pthread_mutex_unlock(&sim->msg_mutex);
		return (errorlog("Malloc failed"), FAILURE);
	}
	new->timestamp = timestamp(sim);
	new->msg = msg;
	new->id = id;
	if (last)
		lock = true;
	if (sim->msg.add(&sim->msg, (void *)new) == FAILURE)
		return (free(new), pthread_mutex_unlock(&sim->msg_mutex), FAILURE);
	pthread_mutex_unlock(&sim->msg_mutex);
	return (SUCCESS);
}
