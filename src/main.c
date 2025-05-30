#include "philo.h"

int main(int argc, char **argv)
{
    t_data   data;
    t_philo  *philos;
    pthread_t *threads;
    int      i;

    /* 1) Parse e validazione degli argomenti */
    if (parse_args(argc, argv, &data) != 0)
    {
        return (EXIT_FAILURE);
    }

    /* 2) Inizializzazione dei dati e dei filosofi */
    if (init_data(&data, &philos) != 0)
    {
        return (EXIT_FAILURE);
    }

    /* 3) Allocazione dell'array di thread */
    threads = malloc(sizeof(pthread_t) * data.n_philo);
    if (!threads)
    {
        perror("malloc threads failed");
        destroy_mutexes(&data);
        free(philos);
        return (EXIT_FAILURE);
    }

    /* 4) Creazione dei thread dei filosofi */
    i = 0;
    while (i < data.n_philo)
    {
        if (pthread_create(&threads[i], NULL,
                           philosopher_routine,
                           &philos[i]) != 0)
        {
            perror("pthread_create failed");
            return (EXIT_FAILURE);
        }
        i++;
    }

    /* 5) Join dei thread */
    i = 0;
    while (i < data.n_philo)
    {
        pthread_join(threads[i], NULL);
        i++;
    }

    /* 6) Cleanup */
    destroy_mutexes(&data);
    free(philos);
    free(threads);

    return (EXIT_SUCCESS);
}
