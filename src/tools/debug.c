/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   debug.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/12 17:51:59 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/14 19:26:22 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "colors.h"

void	debug(const char *s, ...)
{
	va_list	list;

	if (DEBUG == 0)
		return ;
	printf("%s", YELLOW);
	va_start(list, s);
	vprintf(s, list);
	printf("%s", NORMAL);
	va_end(list);
}
