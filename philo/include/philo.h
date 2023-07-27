/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:57 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/27 01:47:45 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef DEBUG
#  define DEBUG 0
# endif

# define ALLOW_ZERO 0
# define PROCESS_LIMIT 2047

# include "../lib/libvector/include/vector.h"

# include <stdbool.h>
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_msg
{
	int					id;
	long				timestamp;
	char				*msg;
}	t_msg;

typedef struct s_value
{
	int					value;

	pthread_mutex_t		mutex;

	int					(*get)(struct s_value *);
	int					(*set)(struct s_value *, int);
	int					(*free)(struct s_value *);
}	t_value;

typedef struct s_fork
{
	int					id;
	pthread_mutex_t		in_use_mutex;
}	t_fork;

typedef struct s_sim
{
	t_v					philos;
	t_v					forks;
	t_v					msg;

	pthread_mutex_t		msg_mutex;
	pthread_mutex_t		time_mutex;

	t_value				start_lock;
	t_value				end_sim;

	int					count;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	long				times_to_eat;
}	t_sim;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	t_sim				*sim;

	t_value				running;
	t_value				x_eaten;

	long				time_to_die;
	pthread_mutex_t		time_to_die_mutex;
}	t_philo;

t_sim	*input(int argc, char **argv, int i);

int		fork_create(t_sim *s);
int		fork_free(void *ptr);

int		philo_create(t_sim *s, int id);
void	*philo_proc(void *ptr);
int		philo_free(void *ptr);

int		simulation(t_sim *s);

bool	everbody_has_eaten(t_sim *sim);
bool	someone_died(t_sim *sim);

int		msg_add(t_sim *sim, int id, char *msg, bool last);
int		msg_print(t_sim *sim);
int		msg_free(void *ptr);

int		value_init(t_value *v);

long	timestamp(t_sim *sim);
void	spend_time(t_sim *sim, long time);

void	*ft_calloc(size_t count, size_t size);
int		ft_strlen(char *str);
void	ft_putint(int num);
bool	errorlog(char *msg);

#endif
