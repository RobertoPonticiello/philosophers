#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

/* Dichiarazione anticipata di t_philo per usarla in t_data */
typedef struct s_philo t_philo;

typedef struct s_data
{
    int             n_philo;
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    int             must_eat_count;

    int             someone_dead;      /* flag usato dal monitor */
    int             full_philos;       /* contatore filosofi “sazi” */

    t_philo         *philos;           /* array di filosofi */
    pthread_mutex_t *forks;            /* array di mutex per le forchette */
    pthread_mutex_t  print_mutex;      /* mutex per serializzare le stampe */
    pthread_mutex_t  data_mutex;       /* mutex per proteggere someone_dead e full_philos */

    long            start_time;       /* timestamp d’inizio simulazione */
}               t_data;

struct s_philo
{
    int             id;
    long            last_meal;        /* momento ultimo pasto (per il monitor) */
    int             eat_count;        /* quante volte ha già mangiato */
    t_data         *data;             /* puntatore alla struct condivisa */
    pthread_t       thread;
};

/* Funzioni di utilità */
int     ft_atoi(const char *str);
void    putnbr_fd(long n, int fd);
size_t  str_len(const char *s);

int     parse_args(int argc, char **argv, t_data *data);
int     init_data(t_data *data, t_philo **philos);
int     init_mutexes(t_data *data);
void    destroy_mutexes(t_data *data);

long    get_timestamp(void);
void    safe_print(t_philo *philo, const char *msg);

void    take_forks(t_philo *p, int left, int right);
void    eat(t_philo *p);
void    release_forks(t_data *d, int left, int right);
void    inc_full(t_philo *p);
void    sleep_and_think(t_philo *p);

void   *philosopher_routine(void *arg);
void   *monitor_routine(void *arg);

#endif