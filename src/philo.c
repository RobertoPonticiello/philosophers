#include "philo.h"

int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		write (2, "Usage: ./philo n_philo time_to_die time_to_eat "
			"time_to_sleep [must_eat_count]\n",
			81);
		return (1);
	}
	data->n_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	if (data->n_philo <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (argc == 6 && data->must_eat_count <= 0))
	{
		write(2, "All arguments must be positive\n", 44);
		return (1);
	}
	return (0);
}

int	init_data(t_data *data, t_philo **philos)
{
	int	i;

	i = 0;
	data->start_time = get_timestamp();
	if (data->start_time < 0)
		return (1);
	*philos = malloc(sizeof(t_philo) * data->n_philo);
	if (!*philos)
		return (1);
	while (i < data->n_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].data = data;
		(*philos)[i].last_meal = data->start_time;
		i++;
	}
	if (init_mutexes(data) != 0)
	{
		free(*philos);
		return (1);
	}
	return (0);
}

void *philosopher_routine(void *arg)
{
	t_philo	*p;
	t_data	*data;
	int		left;
	int		right;

	p = arg;
	data = p->data;
	left = p->id - 1;
	right = (p->id) % data->n_philo;
	if (p->id % 2 == 0)
		usleep(100);
	pthread_mutex_lock(&data->forks[left]);
	safe_print(p, "has taken a fork");
	pthread_mutex_lock(&data->forks[right]);
	safe_print(p, "has taken a fork");
	p->last_meal = get_timestamp();
	safe_print(p, "is eating");
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&data->forks[left]);
	pthread_mutex_unlock(&data->forks[right]);
	return (NULL);
}
