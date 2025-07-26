/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:14:41 by mooumouh          #+#    #+#             */
/*   Updated: 2025/07/26 20:13:00 by mooumouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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

int	ft_help_main(int ac, char **av, t_data *philo, int *i)
{
	if (ac == 6 || ac == 5)
	{
		while (*i < ac)
		{
			if (!ft_check_args(av[(*i)]))
			{
				ft_putstr_fd("try write real number\n", 2);
				return (1);
			}
			(*i)++;
		}
		*philo = arg_philo(ac, av);
		if (!ft_check_nbr(*philo, ac))
		{
			ft_putstr_fd("Invalid Number Issue\n", 2);
			return (1);
		}
	}
	else
	{
		ft_putstr_fd("check your parameters\n", 2);
		return (1);
	}
	return (0);
}

void	ft_sleep(t_philo *philo)
{
	if (philo->data->nbr_philo > 10 && philo->id % 2 != 0)
	{
		if (philo->id % 2 != 0)
			usleep(50000);
	}
	else
	{
		if (philo->id % 2 != 0)
			usleep(500);
	}
}
