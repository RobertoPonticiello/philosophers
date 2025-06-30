#include "philo.h"

void	safe_print(t_philo *philo, const char *msg)
{
	long	ts;

	ts = get_timestamp() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_mutex);
	putnbr_fd(ts, 1);
	write(1, " ", 1);
	putnbr_fd(philo->id, 1);
	write(1, " ", 1);
	write(1, msg, str_len(msg));
	write(1, "\n", 1);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static int	all_philos_full(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->data_mutex);
	result = (data->must_eat_count > 0
			&& data->full_philos == data->n_philo);
	pthread_mutex_unlock(&data->data_mutex);
	return (result);
}

static int	philo_starved(t_philo *philo, long time_to_die)
{
	long	elapsed;
	int		result;
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->data_mutex);
	elapsed = get_timestamp() - philo->last_meal;
	result = (elapsed > time_to_die);
	pthread_mutex_unlock(&data->data_mutex);
	return (result);
}

static int	check_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		if (philo_starved(&data->philos[i], data->time_to_die))
		{
			pthread_mutex_lock(&data->data_mutex);
			safe_print(&data->philos[i], "died");
			data->someone_dead = 1;
			pthread_mutex_unlock(&data->data_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		stop;

	data = (t_data *)arg;
	stop = 0;
	while (!stop)
	{
		if (all_philos_full(data))
		{
			pthread_mutex_lock(&data->data_mutex);
			data->someone_dead = 1;
			pthread_mutex_unlock(&data->data_mutex);
			stop = 1;
		}
		else if (check_philos(data))
			stop = 1;
		else
			usleep(1000);
	}
	return (NULL);
}
