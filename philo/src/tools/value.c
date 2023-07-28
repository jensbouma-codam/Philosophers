/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   value.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:26:15 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/28 10:39:54 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	value_get(t_value *v)
{
	long	value;

	pthread_mutex_lock(&v->mutex);
	value = v->value;
	pthread_mutex_unlock(&v->mutex);
	return (value);
}

static long	value_set(t_value *v, long value)
{
	pthread_mutex_lock(&v->mutex);
	v->value = value;
	pthread_mutex_unlock(&v->mutex);
	return (value);
}

static int	value_free(t_value *v)
{
	if (pthread_mutex_destroy(&v->mutex) != 0)
		return (errorlog("Failed to destroy mutex"), -1);
	return (SUCCESS);
}

int	value_init(t_value *v)
{
	v->value = 0;
	v->set = value_set;
	v->get = value_get;
	v->free = value_free;
	if (pthread_mutex_init(&v->mutex, NULL) != 0)
		return (errorlog("Failed to init mutex"), FAILURE);
	return (SUCCESS);
}
