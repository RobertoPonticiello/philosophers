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

int eat(t_philo *p)
{
    t_data *d   = p->data;
    int     left  = p->id - 1;           /* indice forchetta sinistra   */
    int     right = (p->id) % d->n_philo;/* indice forchetta destra     */

    /* ─────────── Caso n_philo == 1 ─────────── */
    if (d->n_philo == 1)
    {
        pthread_mutex_lock(&d->forks[left]);
        safe_print(p, "has taken a fork");
        usleep(d->time_to_die * 1000);   /* muore stringendo la forchetta */
        pthread_mutex_unlock(&d->forks[left]);
        return (1);                      /* fa uscire la routine          */
    }

    /* ─────────── Prendo le due forchette ─────────── */
    take_forks(p, left, right);          /* stampa due volte "has taken…" */

    /* se nel frattempo qualcuno è morto, esco subito  */
    pthread_mutex_lock(&d->data_mutex);
    if (d->someone_dead)
    {
        pthread_mutex_unlock(&d->data_mutex);
        release_forks(d, left, right);
        return (1);
    }
    /* aggiorno last_meal solo dopo aver preso entrambe le forchette */
    p->last_meal = get_timestamp();
    pthread_mutex_unlock(&d->data_mutex);

    /* ─────────── Mangio ─────────── */
    safe_print(p, "is eating");
    usleep(d->time_to_eat * 1000);

    /* contatore pasti & full_philos */
    pthread_mutex_lock(&d->data_mutex);
    p->eat_count++;
    if (d->must_eat_count > 0 && p->eat_count == d->must_eat_count)
        d->full_philos++;
    pthread_mutex_unlock(&d->data_mutex);

    /* ─────────── Rilascio le forchette ─────────── */
    release_forks(d, left, right);
    return (0);
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

int	sleep_and_think(t_philo *p)
{
	t_data	*d = p->data;

	/* controllo prima di dormire */
	pthread_mutex_lock(&d->data_mutex);
	if (d->someone_dead)
	{
		pthread_mutex_unlock(&d->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&d->data_mutex);

	safe_print(p, "is sleeping");
	usleep(d->time_to_sleep * 1000);

	/* ricontrollo dopo il sonno */
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