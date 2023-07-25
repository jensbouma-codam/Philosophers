/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbouma <jbouma@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 15:23:57 by jbouma        #+#    #+#                 */
/*   Updated: 2023/07/25 02:33:28 by jensbouma     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# ifndef DEBUG
#  define DEBUG 0
# endif

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
}	t_value;

typedef struct s_sim
{
	t_v					philos;
	t_v					msg;
	pthread_mutex_t		msg_mutex;
	bool				msg_lock;
	pthread_mutex_t		mutex;
	int					count;
	long				t_to_die;
	int					t_to_eat;
	int					t_to_sleep;
	int					times_to_eat;
	t_value				proc;
	pthread_t			monitor;
	t_value				everbody_has_eaten;
	t_value				someone_died;
}	t_sim;

typedef struct s_philo
{
	int					id;
	t_value				t_eaten;
	long				time_to_die;
	pthread_mutex_t		time_to_die_mutex;
	pthread_t			thread;
	t_value				running;
	t_sim				*sim;
}	t_philo;

bool	errorlog(char *msg);

t_sim	*input(int argc, char **argv, int i);

long	timestamp(void);
void	spend_time(t_sim *s, int time);

void	*ft_calloc(size_t count, size_t size);
int		ft_strlen(char *str);
void	ft_putint(int num);

int		msg_free(void *ptr);
int		msg_add(t_sim *sim, int id, char *msg, bool last);
int		msg_print(t_sim *sim);

int		philo_free(void *ptr);
void	*philo_proc(void *ptr);
int		philo_create(t_sim *s, int id);

int		value_init(t_value *v);

#endif