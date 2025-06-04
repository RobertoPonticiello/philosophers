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

int init_data(t_data *data, t_philo **philos)
{
    int i;

    i = 0;
    data->start_time   = get_timestamp();
    if (data->start_time < 0)
        return (1);

    /* Azzeriamo i flag e il contatore */
    data->someone_dead = 0;
    data->full_philos  = 0;

    /* Allochiamo l’array di filosofi e lo memorizziamo in data->philos */
    *philos = malloc(sizeof(t_philo) * data->n_philo);
    if (!*philos)
        return (1);
    data->philos = *philos;

    while (i < data->n_philo)
    {
        (*philos)[i].id        = i + 1;
        (*philos)[i].last_meal = data->start_time;
        (*philos)[i].eat_count = 0;
        (*philos)[i].data      = data;
        i++;
    }

    /* Inizializziamo i mutex: forks[], print_mutex e data_mutex */
    if (init_mutexes(data) != 0)
    {
        free(*philos);
        return (1);
    }
    if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
    {
        destroy_mutexes(data);
        free(*philos);
        return (1);
    }
    return (0);
}


void *philosopher_routine(void *arg)
{
    t_philo *p = arg;
    t_data  *d = p->data;
    int      left = p->id - 1;
    int      right = p->id % d->n_philo;

    p->eat_count = 0;
    while (1)
    {
        /* 1) Esco subito se qualcuno è già morto o tutti hanno finito */
        pthread_mutex_lock(&d->data_mutex);
        if (d->someone_dead
            || (d->must_eat_count > 0 && d->full_philos == d->n_philo))
        {
            pthread_mutex_unlock(&d->data_mutex);
            break;
        }
        pthread_mutex_unlock(&d->data_mutex);

        /* 2) Stagger per ID pari */
        if (p->id % 2 == 0)
            usleep(100);

        /* 3) Prendo forchette (uscita anticipata se dead) */
        pthread_mutex_lock(&d->data_mutex);
        if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); break; }
        pthread_mutex_unlock(&d->data_mutex);
        take_forks(p, left, right);

        /* 4) Mangio (uscita anticipata se dead) */
        pthread_mutex_lock(&d->data_mutex);
        if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); break; }
        pthread_mutex_unlock(&d->data_mutex);
        eat(p);

        /* 5) Rilascio forchette */
        release_forks(d, left, right);

        /* 6) Aggiorno contatori “full” se serve */
        if (d->must_eat_count > 0 && p->eat_count == d->must_eat_count)
            inc_full(p);

        /* 7) Dormo e penso (uscita anticipata se dead) */
        pthread_mutex_lock(&d->data_mutex);
        if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); break; }
        pthread_mutex_unlock(&d->data_mutex);
        sleep_and_think(p);
    }
    return (NULL);
}