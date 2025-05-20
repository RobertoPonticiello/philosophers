

typedef struct s_data {
    int             n_philo;
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    int             must_eat_count;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    long            start_time;
}   t_data;

typedef struct s_philo {
    int     id;
    long    last_meal;
    t_data *data;
    pthread_t thread;
}   t_philo;
