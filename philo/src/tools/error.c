/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/12 17:51:59 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/14 19:27:41 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "colors.h"

void	error_exit(char *msg)
{
	if (msg)
		printf("%sError\t%s\n%s", RED, msg, NORMAL);
	exit (EXIT_FAILURE);
}
