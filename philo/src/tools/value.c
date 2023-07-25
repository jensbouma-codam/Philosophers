/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   value.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:26:15 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/25 01:59:16 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	value_get(t_value *v)
{
	int	value;

	pthread_mutex_lock(&v->mutex);
	value = v->value;
	pthread_mutex_unlock(&v->mutex);
	return (value);
}

static int	value_set(t_value *v, int value)
{
	pthread_mutex_lock(&v->mutex);
	v->value = value;
	pthread_mutex_unlock(&v->mutex);
	return (value);
}

int	value_init(t_value *v)
{
	v->value = 0;
	v->set = value_set;
	v->get = value_get;
	pthread_mutex_init(&v->mutex, NULL);
	return (SUCCESS);
}
