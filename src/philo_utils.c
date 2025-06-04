#include "philo.h"

long	get_timestamp(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

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

void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;
    int     i;

    while (1)
    {
        i = 0;
        while (i < data->n_philo)
        {
            pthread_mutex_lock(&data->data_mutex);
            /* Se tutti hanno mangiato abbastanza volte, esco */
            if (data->must_eat_count > 0
                && data->full_philos == data->n_philo)
            {
                pthread_mutex_unlock(&data->data_mutex);
                return (NULL);
            }
            /* Se un filosofo è scaduto, stampo “died” e metto someone_dead */
            if (get_timestamp() - data->philos[i].last_meal
                > data->time_to_die)
            {
                safe_print(&data->philos[i], "died");
                data->someone_dead = 1;
                pthread_mutex_unlock(&data->data_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&data->data_mutex);
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}
