/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:57 by jbouma        #+#    #+#                 */
/*   Updated: 2023/06/21 03:33:29 by jensbouma     ########   odam.nl         */
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

enum e_state
{
	NORMAL,
	JOINING,
	DEAD
};

struct s_msg_list
{
	long				ts;
	uint32_t			id;
	char				*msg;
	struct s_msg_list	*next;
};

struct s_msg
{
	pthread_mutex_t		mutex;
	struct s_msg_list	*msg;
	struct s_msg_list	*last;
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
	bool			seat_taken;
	int32_t			times_eaten;
	pthread_t		philosopher;
	struct s_table	*next;
	struct s_table	*prev;
};

struct s_simulation
{
	pthread_mutex_t		mutex;
	uint32_t			philosophers;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int32_t				must_eat;
	struct s_msg		*printer;
	struct s_table		*table;
};

struct s_simulation	*input(int argc, char **argv);

void				*philo_lifecycle(void *arg);

struct s_table		*table_cutlery(int i);
void				table_join(struct s_simulation *sim);
void				table_mutex_init(struct s_simulation *sim);
void				table_tread_create(struct s_simulation *sim);	

int					msg_print(struct s_msg *p);
void				msg_add(struct s_msg *p, uint32_t id, char *msg);

bool				watch_them_die(struct s_simulation *sim);	

long				timestamp(void);	

void				error_exit(char *msg);	

void				debug(const char *s, ...);	

void				*mem_add(size_t count, size_t size);

#endif