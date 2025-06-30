#include "philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

int	create_philosophers(t_data *data, t_philo *philos, pthread_t *threads)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < data->n_philo)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine, &philos[i]))
			ret = 1;
		i++;
	}
	return (ret);
}

void	join_philosophers(t_data *data, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int	start_threads(t_data *data, t_philo *philos, pthread_t *threads)
{
	pthread_t	monitor;

	if (create_philosophers(data, philos, threads))
		return (1);
	if (pthread_create(&monitor, NULL, monitor_routine, data))
		return (1);
	pthread_join(monitor, NULL);
	join_philosophers(data, threads);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	*threads;
	int			ret;

	if (parse_args(argc, argv, &data))
		return (1);
	if (init_data(&data, &philos))
		return (1);
	threads = malloc(sizeof(pthread_t) * data.n_philo);
	if (!threads)
		return (1);
	ret = start_threads(&data, philos, threads);
	destroy_mutexes(&data);
	free(philos);
	free(threads);
	return (ret);
}
