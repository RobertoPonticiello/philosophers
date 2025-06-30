#include "philo.h"

int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		write (2, "Usage: ./philo n_philo time_to_die time_to_eat "
			"time_to_sleep [must_eat_count]\n",
			79);
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
		write(2, "All arguments must be positive\n", 32);
		return (1);
	}
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*p;
	t_data	*d;

	p = (t_philo *)arg;
	d = p->data;
	if (p->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&d->data_mutex);
		if (d->someone_dead)
		{
			pthread_mutex_unlock(&d->data_mutex);
			break ;
		}
		pthread_mutex_unlock(&d->data_mutex);
		if (eat(p))
			break ;
		if (sleep_and_think(p))
			break ;
	}
	return (NULL);
}
