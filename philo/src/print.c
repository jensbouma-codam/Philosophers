/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/20 22:35:49 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/11 16:22:56 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	msg_print(struct s_msg_queue *p)
{
	struct s_msg	*msg;

	if (pthread_mutex_lock(&p->mutex) != 0)
		return (error_exit("mutex lock failed\n"));
	msg = p->msg;
	while (msg)
	{
		printf("%ld %d %s\n", msg->timestamp / 1000, msg->id, msg->msg);
		free(msg);
		msg = msg->next;
	}
	p->msg = NULL;
	p->last = NULL;
	pthread_mutex_unlock(&p->mutex);
	return (0);
}

char	*str_cpy(char *src)
{
	int		i;
	char	*dst;

	i = 0;
	while (src[i])
		++i;
	dst = mem_add(i + 1, sizeof(char));
	if (!dst)
		return (NULL);
	dst[i--] = '\0';
	while (src[i])
	{
		dst[i] = src[i];
		--i;
	}
	return (dst);
}

bool	msg_add(struct s_msg_queue *p, uint32_t id, char *msg)
{
	struct s_msg	*new;

	new = mem_add(1, sizeof(struct s_msg));
	if (!new)
		return (EXIT_FAILURE);
	new->timestamp = timestamp();
	new->msg = msg;
	new->id = id;
	new->next = NULL;
	if (pthread_mutex_lock(&p->mutex) != 0)
		return (free(new), EXIT_FAILURE);
	if (p->msg == NULL)
	{
		p->msg = new;
		p->last = new;
	}
	else
	{
		p->last->next = new;
		p->last = new;
	}
	pthread_mutex_unlock(&p->mutex);
	return (EXIT_SUCCESS);
}
