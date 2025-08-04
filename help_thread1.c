/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_thread1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:22:21 by mooumouh          #+#    #+#             */
/*   Updated: 2025/08/04 11:00:06 by mooumouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_action(t_philo *philo, char *msg)
{
	t_data	*data;
	int		died;

	data = philo->data;
	pthread_mutex_lock(&data->death_lock);
	pthread_mutex_lock(&data->print_lock);
	died = data->s_died;
	if (!died)
		printf("%ld %d %s\n", get_time_ms() - data->time, philo->id, msg);
	pthread_mutex_unlock(&data->death_lock);
	pthread_mutex_unlock(&data->print_lock);
}

void	index_philsophers(t_data *data)
{
	int	i;

	i = 0;
	data->time = get_time_ms();
	data->s_died = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	while (i < data->nbr_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->nbr_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->full_philos = 0;
		data->philos[i].last_meal_time = data->time;
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[(i + 1) % data->nbr_philo];
		data->philos[i].data = data;
		i++;
	}
}

void	free_thread(t_data *philo, int *limit)
{
	int	i;

	i = 0;
	while (i < *limit)
	{
		pthread_join(philo->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < philo->nbr_philo)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->print_lock);
	pthread_mutex_destroy(&philo->meal_lock);
	pthread_mutex_destroy(&philo->death_lock);
}

void	forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->r_fork);
	print_action(philo, "has taken a fork");
}
