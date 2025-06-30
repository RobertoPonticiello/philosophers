#include "philo.h"

int	init_philosophers(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		philos[i].id = i + 1;
		philos[i].last_meal = data->start_time;
		philos[i].eat_count = 0;
		philos[i].data = data;
		i++;
	}
	return (0);
}

int	init_all_mutexes(t_data *data, t_philo *philos)
{
	if (init_mutexes(data) != 0)
	{
		free(philos);
		return (1);
	}
	if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
	{
		destroy_mutexes(data);
		free(philos);
		return (1);
	}
	return (0);
}

int	init_data(t_data *data, t_philo **philos)
{
	long	start_time;

	start_time = get_timestamp();
	if (start_time < 0)
		return (1);
	data->start_time = start_time;
	data->someone_dead = 0;
	data->full_philos = 0;
	*philos = malloc(sizeof(t_philo) * data->n_philo);
	if (!(*philos))
		return (1);
	data->philos = *philos;
	if (init_philosophers(data, *philos))
	{
		free(*philos);
		return (1);
	}
	if (init_all_mutexes(data, *philos))
		return (1);
	return (0);
}
