/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/20 22:35:49 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/21 02:40:29 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	msg_print(struct s_msg *p)
{
	struct s_msg_list	*msg;
	struct s_msg_list	*next;
	int					i;

	if (pthread_mutex_lock(&p->mutex) != 0)
		error_exit("mutex lock failed\n");
	msg = p->msg;
	i = 0;
	while (msg)
	{
		next = msg->next;
		printf("%ld %d %s\n", msg->ts / 1000, msg->id, msg->msg);
		free(msg->msg);
		msg = next;
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

	while (src[i])
		i++;
	dst = mem_add(i + 1, sizeof(char));
	dst[i--] = '\0';
	while (src[i])
	{
		dst[i] = src[i];
		i--;
	}
	return (dst);
}

void	msg_add(struct s_msg *p, uint32_t id, char *msg)
{
	struct s_msg_list	*new;

	if (pthread_mutex_lock(&p->mutex) != 0)
		error_exit("mutex lock failed\n");
	new = mem_add(1, sizeof(struct s_msg_list));
	new->ts = timestamp();
	new->msg = str_cpy(msg);
	new->id = id;
	new->next = NULL;
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
}
