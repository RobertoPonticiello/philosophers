#include "philo.h"

int init_mutexes(t_data *data)
{
    int i;

    data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philo);
    if (!data->forks)
        return (1);

    i = 0;
    while (i < data->n_philo)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (1);
        i++;
    }

    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
        return (1);

    /* Non inizializziamo qui data_mutex: lo fa init_data */
    return (0);
}


void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->data_mutex);
	free(data->forks);
}
