/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mooumouh <mooumouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:41:38 by mooumouh          #+#    #+#             */
/*   Updated: 2025/07/16 13:27:39 by mooumouh         ###   ########.fr       */
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
	return (1);
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
