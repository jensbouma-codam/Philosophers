/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/12 17:51:59 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/10 12:44:44 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "colors.h"

/**
 * @brief Print error message and exit program.
 * 
 * @param msg 
 */
void	error_exit(char *msg)
{
	if (msg)
		printf("%sError\t%s\n%s", RED, msg, NORMAL);
	exit (EXIT_FAILURE);
}
