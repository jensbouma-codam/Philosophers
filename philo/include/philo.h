/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:57 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/11 16:58:28 by jbouma        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef DEBUG
#  define DEBUG 0
# endif

# define MAX_THREATS 709

# include <stdbool.h>
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>

struct s_msg
{
	long			timestamp;
	uint32_t		id;
	char			*msg;
	struct s_msg	*next;
};

struct s_msg_queue
{
	pthread_mutex_t	mutex;
	struct s_msg	*msg;
	struct s_msg	*last;
};

struct s_fork
{
	pthread_mutex_t	mutex;
	bool			in_use;
};

struct s_table
{
	pthread_t		philosopher;
	uint32_t		id;
	struct s_fork	*l_fork;
	struct s_fork	*r_fork;
	long			dead;
	int32_t			times_eaten;
	bool			seat_taken;
	bool			is_eating;
	struct s_table	*next;
	struct s_table	*prev;
};

struct s_simulation
{
	long				start;
	uint32_t			philosophers;
	uint32_t			time_to_die;
	uint32_t			time_to_eat;
	uint32_t			time_to_sleep;
	int32_t				times_to_eat;
	struct s_msg_queue	*msg_queue;
	struct s_table		*table;
};

struct s_simulation	*input(int argc, char **argv);

void				*philo_lifecycle(void *arg);

struct s_table		*table_add_cutlery(int i);
void				table_add_seats(struct s_simulation *sim);
void				table_add_mutexes(struct s_simulation *sim);
void				philo_join_table(struct s_simulation *sim);	

int					msg_print(struct s_msg_queue *p);
bool				msg_add(struct s_msg_queue *p, uint32_t id, char *msg);

bool				watch_them_die(struct s_simulation *sim);	

long				timestamp(void);	

bool				error_exit(char *msg);	

void				debug(const char *s, ...);	

void				*mem_add(size_t count, size_t size);

#endif