#include "philo.h"
/*
 * take_forks: blocca prima una forchetta e poi l'altra,
 *             con stampa “has taken a fork” per ciascuna.
 */
void take_forks(t_philo *p, int left, int right)
{
    t_data *d = p->data;

    /* Prendo forchetta “left” */
    pthread_mutex_lock(&d->forks[left]);
    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); return; }
    pthread_mutex_unlock(&d->data_mutex);
    safe_print(p, "has taken a fork");

    /* Prendo forchetta “right” */
    pthread_mutex_lock(&d->forks[right]);
    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); return; }
    pthread_mutex_unlock(&d->data_mutex);
    safe_print(p, "has taken a fork");
}

/*
 * eat: aggiorna last_meal, stampa “is eating”, 
 *      dorme per time_to_eat ms e incrementa eat_count.
 */
void eat(t_philo *p)
{
    t_data *d = p->data;

    /* Aggiorno last_meal */
    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); return; }
    p->last_meal = get_timestamp();
    pthread_mutex_unlock(&d->data_mutex);

    /* Stampo “is eating” e dormo */
    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); return; }
    pthread_mutex_unlock(&d->data_mutex);
    safe_print(p, "is eating");
    usleep(d->time_to_eat * 1000);

    /* Incremento il contatore di pasti */
    p->eat_count++;
}

/*
 * release_forks: sblocca le due forchette.
 */
void release_forks(t_data *d, int left, int right)
{
    pthread_mutex_unlock(&d->forks[left]);
    pthread_mutex_unlock(&d->forks[right]);
}
/*
 * inc_full: incrementa full_philos in modo thread-safe
 */
void inc_full(t_philo *p)
{
    t_data *d = p->data;

    pthread_mutex_lock(&d->data_mutex);
    d->full_philos++;
    pthread_mutex_unlock(&d->data_mutex);
}

/*
 * sleep_and_think: stampa “is sleeping”, dorme, poi “is thinking”
 */

void sleep_and_think(t_philo *p)
{
    t_data *d = p->data;

    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); return; }
    pthread_mutex_unlock(&d->data_mutex);
    safe_print(p, "is sleeping");
    usleep(d->time_to_sleep * 1000);

    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead) { pthread_mutex_unlock(&d->data_mutex); return; }
    pthread_mutex_unlock(&d->data_mutex);
    safe_print(p, "is thinking");
}

