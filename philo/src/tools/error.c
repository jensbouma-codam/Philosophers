/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/12 17:51:59 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/11 17:06:52 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "colors.h"

/**
 * @brief Print error message and exit program.
 * 
 * @param msg 
 */
bool	errorlog(char *msg)
{
	if (msg)
		printf("%sError\t%s\n%s", RED, msg, NORMAL);
	return (EXIT_FAILURE);
}
