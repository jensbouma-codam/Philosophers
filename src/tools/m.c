/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   m.c                                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 17:01:34 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/12 17:12:28 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*m_add(size_t count, size_t size)
{
	char	*ptr;
	int		n;

	n = size * count;
	ptr = malloc(n + 1);
	if (!ptr)
		c_error("Insane in the membrane! Insane in the brain!");
	while (n >= 0)
		ptr[n--] = '\0';
	return ((void *) ptr);
}
