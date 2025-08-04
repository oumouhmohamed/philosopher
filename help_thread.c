/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:48:30 by mooumouh          #+#    #+#             */
/*   Updated: 2025/07/30 13:45:59 by mooumouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	handle_sleep(long time_ms, t_data *data)
{
	long	start;
	int		died;

	pthread_mutex_lock(&data->death_lock);
	died = data->s_died;
	pthread_mutex_unlock(&data->death_lock);
	start = get_time_ms();
	while (!died && (get_time_ms() - start < time_ms))
	{
		usleep(100);
		pthread_mutex_lock(&data->death_lock);
		died = data->s_died;
		pthread_mutex_unlock(&data->death_lock);
	}
}

void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_action(philo, "is eating");
	pthread_mutex_lock(&data->meal_lock);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&data->meal_lock);
	handle_sleep(data->time_eat, data);
	philo->meals_eaten++;
	if (data->nbr_times_philo_must_eat != -1
		&& philo->meals_eaten == data->nbr_times_philo_must_eat)
	{
		pthread_mutex_lock(&data->meal_lock);
		data->full_philos++;
		pthread_mutex_unlock(&data->meal_lock);
	}
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	one_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(philo->l_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->death_lock);
	while (!philo->data->s_died)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		usleep(100);
		pthread_mutex_lock(&philo->data->death_lock);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(philo->l_fork);
}

void	*action_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	ft_sleep(philo);
	if (philo->data->nbr_philo == 1)
		one_philo(philo);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		if (philo->data->s_died)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_lock);
		print_action(philo, "is thinking");
		forks(philo);
		eat(philo);
		drop_forks(philo);
		print_action(philo, "is sleeping");
		handle_sleep(philo->data->time_sleep, philo->data);
	}
	return (NULL);
}
