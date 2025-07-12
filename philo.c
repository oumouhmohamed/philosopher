/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:06:06 by mooumouh          #+#    #+#             */
/*   Updated: 2025/07/12 01:25:23 by mooumouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

long	get_time_ms(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
		data->philos[i].last_meal_time = data->time;
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[(i + 1) % data->nbr_philo];
		data->philos[i].data = data;
		i++;
	}
}
long	ft_atoi(const char *str, int *len)
{
	int		i;
	long	result;

	i = 0;
	*len = 0;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9') && *len < 12)
	{
		result = result * 10 + (str[i] - '0');
		if (result)
			(*len)++;
		i++;
	}
	if (result == 0)
		result = -1;
	if (result > 2147483647)
		return (-1);
	return (result);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s || fd < 0)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	ft_check_nbr(t_data list_philo, int ac)
{
	if (list_philo.nbr_philo > 200)
		return (0);
	if (list_philo.nbr_philo == -1 || list_philo.time_die == -1
			|| list_philo.time_eat == -1 || list_philo.time_sleep == -1)
				return (0);
	if (list_philo.time_die < 60
			|| list_philo.time_eat < 60 || list_philo.time_sleep < 60)
				return (0);
	if (ac == 6)
	{
		if (list_philo.nbr_times_philo_must_eat == -1)
			return (0);
	}
	return(1);
}

void	safe_print(t_philo *philo, char *msg)
{
	t_data	*data;
	int		died;
	
	data = philo->data;
	pthread_mutex_lock(&data->death_lock);
	died = data->s_died;
	pthread_mutex_unlock(&data->death_lock);
	pthread_mutex_lock(&data->print_lock);
	if (!died)
		printf("%ld %d %s\n", get_time_ms() - data->time, philo->id, msg);
	pthread_mutex_unlock(&data->print_lock);
}

void	forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		safe_print(philo, "take fork");
		pthread_mutex_lock(philo->r_fork);
		safe_print(philo, "take fork");
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		safe_print(philo, "take fork");
		pthread_mutex_lock(philo->l_fork);
		safe_print(philo, "take fork");
	}
}

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
	t_data *data = philo->data;

	safe_print(philo, "is eating");
	pthread_mutex_lock(&data->meal_lock);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&data->meal_lock);
	philo->meals_eaten++;
	if (data->nbr_times_philo_must_eat != -1 && philo->meals_eaten == data->nbr_times_philo_must_eat)
	{
		pthread_mutex_lock(&data->meal_lock);
		data->full_philos++;
		pthread_mutex_unlock(&data->meal_lock);
	}
	handle_sleep(data->time_eat, data);
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
	safe_print(philo, "take fork");
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
	t_philo *philo = (t_philo *)arg;

	if (philo->id % 2 == 0)
		usleep(1000);
	if(philo->data->nbr_philo == 1)
		one_philo(philo);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		if (philo->data->s_died)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
			break;
		}
		pthread_mutex_unlock(&philo->data->death_lock);
		forks(philo);
		eat(philo);
		drop_forks(philo);
		safe_print(philo, "is sleeping");
		handle_sleep(philo->data->time_sleep, philo->data);
		safe_print(philo, "is thinking");
	}
	return (NULL);
}

t_data	arg_philo(int ac, char **av)
{
	t_data	list_philo;
	int		len;

	len = 0;
	list_philo.nbr_philo = ft_atoi(av[1], &len);
	list_philo.time_die = ft_atoi(av[2], &len);
	list_philo.time_eat = ft_atoi(av[3], &len);
	list_philo.time_sleep = ft_atoi(av[4], &len);
	if (ac == 6)
		list_philo.nbr_times_philo_must_eat = ft_atoi(av[5], &len);
	else
		list_philo.nbr_times_philo_must_eat = -1;
	return (list_philo);
}

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

int	ft_check_args(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int main(int ac, char **av)
{
	int			i;
	pthread_t	death_monitor;
	t_data		philo;

	i = 1;
	if (ac == 6 || ac == 5)
	{
		while (i < ac)
		{
			if (!ft_check_args(av[i]))
			{
				ft_putstr_fd("try write real number\n", 2);
				return(1);
			}
			i++;
		}
		philo = arg_philo(ac, av);
		if (!ft_check_nbr(philo, ac))
		{
			ft_putstr_fd("error\n", 2);
			return (1);
		}
	}
	else
	{
		ft_putstr_fd("error\n", 2);
		return (1);
	}
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
	pthread_create(&death_monitor, NULL, monitor, &philo);
	i = 0;
	while (i < philo.nbr_philo)
	{
		pthread_join(philo.philos[i].thread, NULL);
		i++;
	}
	pthread_join(death_monitor, NULL);
	i = 0;
	while (i < philo.nbr_philo)
	{
		pthread_mutex_destroy(&philo.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&philo.print_lock);
	pthread_mutex_destroy(&philo.meal_lock);
	return (0);
}
