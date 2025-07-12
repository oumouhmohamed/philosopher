/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:37:35 by mooumouh          #+#    #+#             */
/*   Updated: 2025/07/12 13:26:53 by mooumouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*monitor(void *arg)
{
	t_data	*data = (t_data *)arg;
	long	time_since_meal;
	long	curent;
	int		i;

	i = 0;
	curent = 0;
	while (!data->s_died)
	{
		i = 0;
		while (i < data->nbr_philo)
		{
			curent = get_time_ms();
			pthread_mutex_lock(&data->meal_lock);
			time_since_meal = curent - data->philos[i].last_meal_time;
			pthread_mutex_unlock(&data->meal_lock);
			if (time_since_meal > data->time_die)
			{
				pthread_mutex_lock(&data->print_lock);
				printf("%ld %d died\n", get_time_ms() - data->time, data->philos[i].id);
				pthread_mutex_lock(&data->death_lock);
				data->s_died = 1;
				pthread_mutex_unlock(&data->death_lock);
				pthread_mutex_unlock(&data->print_lock);
				return (NULL);
			}
			i++;
		}
		pthread_mutex_lock(&data->meal_lock);
		if (data->nbr_times_philo_must_eat != -1 && data->full_philos >= data->nbr_philo)
		{
			pthread_mutex_lock(&data->death_lock);
				data->s_died = 1;
			pthread_mutex_unlock(&data->death_lock);
			pthread_mutex_unlock(&data->meal_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&data->meal_lock);
		usleep(1000);
	}
	return (NULL);
}

int main(int ac, char **av)
{
	int			i;
	t_data		philo;

	i = 1;
	if (ft_help_main(ac, av, &philo, &i) == 1)
		return(1);	
	index_philsophers(&philo);
	i = 0;
	while (i < philo.nbr_philo)
	{
		if (pthread_create(&philo.philos[i].thread, NULL, action_philo, &philo.philos[i]) != 0)
		{
			ft_putstr_fd("error\n", 2);
			return (1);
		}
		i++;
	}
	pthread_create(&philo.death_monitor, NULL, monitor, &philo);
	free_thread(&philo);
	return (0);
}
