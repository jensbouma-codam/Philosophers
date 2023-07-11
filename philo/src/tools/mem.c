/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mem.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 17:01:34 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 17:07:34 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Allocate memory and initialize it to zero.
 * If malloc fails, exit program.
 * 
 * @param count 
 * @param size 
 * @return void* 
 */
void	*ft_calloc(size_t count, size_t size)
{
	char	*ptr;
	int		n;

	n = size * count;
	ptr = malloc(n + 1);
	if (!ptr)
	{
		errorlog("Insane in the membrane! Insane in the brain!");
		return (NULL);
	}
	while (n >= 0)
		ptr[n--] = '\0';
	return ((void *) ptr);
}
