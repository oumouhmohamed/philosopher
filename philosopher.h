/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:14:17 by mooumouh          #+#    #+#             */
/*   Updated: 2025/07/12 22:17:09 by mooumouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>

# define MAX_PHILO 200

typedef struct s_data	t_data;

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
	pthread_t		death_monitor;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	forks[MAX_PHILO];
	pthread_mutex_t	print_lock;
	t_philo			philos[MAX_PHILO];
}					t_data;

int		ft_strlen(const char *s);
long	ft_atoi(const char *str, int *len);
void	ft_putstr_fd(char *s, int fd);
int		ft_check_nbr(t_data list_philo, int ac);
int		ft_check_args(char *str);
t_data	arg_philo(int ac, char **av);
int		ft_help_main(int ac, char **av, t_data *philo, int *i);
void	handle_sleep(long time_ms, t_data *data);
void	eat(t_philo *philo);
void	drop_forks(t_philo *philo);
void	one_philo(t_philo *philo);
void	*action_philo(void *arg);
long	get_time_ms(void);
void	safe_print(t_philo *philo, char *msg);
void	forks(t_philo *philo);
void	index_philsophers(t_data *data);
void	free_thread(t_data *philo);
#endif
