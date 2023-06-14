/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   c.c                                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/12 17:51:59 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/06/14 18:30:34 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "colors.h"

static void	printf_wrapper(const char *s, va_list list)
{
	while (*s)
	{
		if (*s == '%' && *s++ && *s != '%')
		{
			if (*s == 'c')
				printf("%c", va_arg(list, int));
			else if (*s == 'd')
				printf("%d", va_arg(list, int));
			else if (*s == 'u')
				printf("%u", va_arg(list, unsigned int));
			else if (*s == 's')
				printf("%s", va_arg(list, char *));
		}
		else
			printf("%c", *s);
		s++;
	}
}

void	c_error(char *msg)
{
	if (msg)
		printf("%sError\t%s\n%s", RED, msg, NORMAL);
	exit (EXIT_FAILURE);
}

void	c_log(const char *s, ...)
{
	va_list	list;

	va_start(list, s);
	printf_wrapper(s, list);
	va_end(list);
}

void	c_debug(const char *s, ...)
{
	va_list	list;

	if (DEBUG == 0)
		return ;
	printf("%s", YELLOW);
	va_start(list, s);
	printf_wrapper(s, list);
	printf("%s", NORMAL);
	va_end(list);
}
