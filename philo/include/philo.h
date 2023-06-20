/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:57 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/20 02:20:55 by jensbouma     ########   odam.nl         */
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
# include <pthread.h>

struct s_arg
{
	uint32_t	philosophers;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	uint32_t	must_eat;
};

enum e_state
{
	JOINING,
	THINKING,
	EATING,
	SLEEPING,
	DEAD
};

struct s_fork
{
	pthread_mutex_t	mutex;
	bool			in_use;
};

struct s_table
{
	uint32_t		id;
	struct s_fork	*l_fork;
	struct s_fork	*r_fork;
	pthread_t		philosopher;
	struct s_table	*next;
	struct s_table	*prev;
	struct s_arg	*arg;
	long			dead_date;
	uint32_t		times_eaten;
	int				state;
	bool			dead;
	bool			took_forks;
	int				old_state;
};

struct s_arg	*input(int argc, char **argv);

bool			watch_them_die(struct s_table *table);
void			*philo_lifecycle(void *arg);
bool			philo_eat_sleep(struct s_table *table, struct s_arg *a);

struct s_table	*table_cutlery(int i);
void			table_join(struct s_table *table, struct s_arg *a);
void			table_mutex_init(struct s_table *table, struct s_arg *a);
void			table_tread_create(struct s_table *t);

void			print_state(struct s_table *t, long ts);
bool			watch_die(struct s_table *table);

long			timestamp(void);

void			error_exit(char *msg);

void			debug(const char *s, ...);

void			*mem_add(size_t count, size_t size);

#endif