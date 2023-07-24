/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/20 22:35:49 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/24 16:07:12 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	put_str(char *str)
{
	write(1, str, strlen(str));
}

void	put_num(int num)
{
	char	c;

	if (num < 0)
	{
		write(1, "-", 1);
		num = -num;
	}
	if (num / 10)
		put_num(num / 10);
	c = num % 10 + '0';
	write(1, &c, 1);
}

void	printmsg(struct s_msg *msg)
{
	// write(1, "\033[0;32m", 8);
	put_num(msg->timestamp / 1000);
	write(1, " ", 1);
	// write(1, "\033[0;33m", 8);
	put_num(msg->id);
	write(1, " ", 1);
	// write(1, "\033[0;36m", 8);
	put_str(msg->msg);
	write(1, "\n", 1);
}

int	msg_print(struct s_msg_queue *p)
{
	struct s_msg	*msg;

	if (pthread_mutex_lock(&p->mutex) != 0)
		return (errorlog("mutex lock failed\n"));
	msg = p->msg;
	while (msg)
	{
		printmsg(msg);
		msg = msg->next;
	}
	msg = p->msg;
	while (msg)
	{
		free(msg);
		msg = msg->next;
	}
	p->msg = NULL;
	p->last = NULL;
	pthread_mutex_unlock(&p->mutex);
	return (0);
}

bool	msg_add(struct s_msg_queue *p, uint32_t id, char *msg)
{
	struct s_msg	*new;

	new = ft_calloc(1, sizeof(struct s_msg));
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
