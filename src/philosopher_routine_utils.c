#include "philo.h"

void	take_forks(t_philo *p, int left, int right)
{
	t_data	*d;

	d = p->data;
	pthread_mutex_lock(&d->forks[left]);
	pthread_mutex_lock(&d->data_mutex);
	if (d->someone_dead)
	{
		pthread_mutex_unlock(&d->data_mutex);
		return ;
	}
	pthread_mutex_unlock(&d->data_mutex);
	safe_print(p, "has taken a fork");
	pthread_mutex_lock(&d->forks[right]);
	pthread_mutex_lock(&d->data_mutex);
	if (d->someone_dead)
	{
		pthread_mutex_unlock(&d->data_mutex);
		return ;
	}
	pthread_mutex_unlock(&d->data_mutex);
	safe_print(p, "has taken a fork");
}

static int	eat_alone(t_philo *p, int left)
{
	t_data	*d;

	d = p->data;
	pthread_mutex_lock(&d->forks[left]);
	safe_print(p, "has taken a fork");
	usleep(d->time_to_die * 1000);
	pthread_mutex_unlock(&d->forks[left]);
	return (1);
}

static int	start_eating(t_philo *p, int left, int right)
{
	t_data	*d;
	int		dead;

	d = p->data;
	take_forks(p, left, right);
	pthread_mutex_lock(&d->data_mutex);
	dead = d->someone_dead;
	if (!dead)
		p->last_meal = get_timestamp();
	pthread_mutex_unlock(&d->data_mutex);
	if (dead)
	{
		release_forks(d, left, right);
		return (1);
	}
	return (0);
}

static void	finish_eating(t_philo *p, int left, int right)
{
	t_data	*d;

	d = p->data;
	safe_print(p, "is eating");
	usleep(d->time_to_eat * 1000);
	pthread_mutex_lock(&d->data_mutex);
	p->eat_count++;
	if (d->must_eat_count > 0 && p->eat_count == d->must_eat_count)
		d->full_philos++;
	pthread_mutex_unlock(&d->data_mutex);
	release_forks(d, left, right);
}

int	eat(t_philo *p)
{
	t_data	*d;
	int		left;
	int		right;

	d = p->data;
	left = p->id - 1;
	right = (p->id) % d->n_philo;
	if (d->n_philo == 1)
		return (eat_alone(p, left));
	if (start_eating(p, left, right))
		return (1);
	finish_eating(p, left, right);
	return (0);
}
