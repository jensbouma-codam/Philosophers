/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jensbouma <jensbouma@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 23:29:57 by jensbouma     #+#    #+#                 */
/*   Updated: 2023/07/24 23:34:09 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putint(int num)
{
	char	c;

	if (num < 0)
	{
		write(1, "-", 1);
		num = -num;
	}
	if (num / 10)
		ft_putint(num / 10);
	c = num % 10 + '0';
	write(1, &c, 1);
}
