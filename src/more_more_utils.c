#include "philo.h"

void	release_forks(t_data *d, int left, int right)
{
	pthread_mutex_unlock(&d->forks[left]);
	pthread_mutex_unlock(&d->forks[right]);
}

void	inc_full(t_philo *p)
{
	t_data	*d;

	d = p->data;
	pthread_mutex_lock(&d->data_mutex);
	d->full_philos++;
	pthread_mutex_unlock(&d->data_mutex);
}

int	sleep_and_think(t_philo *p)
{
	t_data	*d;

	d = p->data;
	pthread_mutex_lock(&d->data_mutex);
	if (d->someone_dead)
	{
		pthread_mutex_unlock(&d->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&d->data_mutex);
	safe_print(p, "is sleeping");
	usleep(d->time_to_sleep * 1000);
	pthread_mutex_lock(&d->data_mutex);
	if (d->someone_dead)
	{
		pthread_mutex_unlock(&d->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&d->data_mutex);
	safe_print(p, "is thinking");
	return (0);
}
