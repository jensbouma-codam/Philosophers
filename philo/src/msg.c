/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   msg.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/20 22:35:49 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/31 12:00:57 by jensbouma     ########   odam.nl         */
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
		ft_putint(msg->id + 1);
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
	if (sim->msg.size(&sim->msg) == 0)
		return (pthread_mutex_unlock(&sim->msg_mutex), FAILURE);
	while (i < sim->msg.size(&sim->msg))
	{
		msg = (t_msg *)sim->msg.get(&sim->msg, i++);
		printmsg(msg);
	}
	while (i--)
		sim->msg.del(&sim->msg, i);
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

	new = ft_calloc(1, sizeof(t_msg));
	if (!new)
		return (errorlog("Failed to add message"), FAILURE);
	new->id = id;
	new->msg = msg;
	pthread_mutex_lock(&sim->msg_mutex);
	if (lock && id != -1)
		return (free(new), pthread_mutex_unlock(&sim->msg_mutex), SUCCESS);
	if (last)
		lock = true;
	new->timestamp = timestamp(sim);
	if (sim->msg.add(&sim->msg, new) == FAILURE)
		return (free(new), pthread_mutex_unlock(&sim->msg_mutex), FAILURE);
	pthread_mutex_unlock(&sim->msg_mutex);
	return (SUCCESS);
}
