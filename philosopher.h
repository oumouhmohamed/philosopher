#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define Max_philo 200

typedef struct s_data t_data;

typedef struct s_philo {
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				nbr_philo;
	int				time_eat;
	int				time_die;
	int				time_sleep;
	int				nbr_times_philo_must_eat;
	long			time;
	int				s_died;
	int				full_philos;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	forks[Max_philo];
	pthread_mutex_t	print_lock;
	t_philo			philos[Max_philo];
}					t_data;

#endif
