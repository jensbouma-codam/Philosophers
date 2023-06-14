/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:57 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/14 22:00:47 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef DEBUG
#  define DEBUG 0
# endif

# include <stdbool.h>
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>

struct s_arg
{
	int			philosophers;
	uint32_t	time_to_die;
	uint32_t	time_to_eat;
	uint32_t	time_to_sleep;
	uint32_t	must_eat;
};

struct s_fork
{
	bool	in_use;
};

enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD
};

struct s_philo
{
	int		id;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		state;
};

struct s_table
{
	struct s_fork	*l_fork;
	struct s_fork	*r_fork;
	struct s_philo	*philosopher;
	struct s_table	*next;
	struct s_table	*prev;
};

struct s_arg	input(int argc, char **argv);

bool			simulation(struct s_table *table, struct s_arg a);

struct s_table	*table_cutlery(int i);
void			table_join(struct s_table *table, struct s_arg a);

void			error_exit(char *msg);

void			debug(const char *s, ...);

void			*mem_add(size_t count, size_t size);

#endif