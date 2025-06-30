#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				n_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	int				someone_dead;
	int				full_philos;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	long			start_time;
}	t_data;

struct s_philo
{
	int			id;
	long		last_meal;
	int			eat_count;
	t_data		*data;
	pthread_t	thread;
};

int		ft_atoi(const char *str);
void	putnbr_fd(long n, int fd);
size_t	str_len(const char *s);
int		parse_args(int argc, char **argv, t_data *data);
int		init_data(t_data *data, t_philo **philos);
int		init_mutexes(t_data *data);
void	destroy_mutexes(t_data *data);
long	get_timestamp(void);
void	safe_print(t_philo *philo, const char *msg);
void	take_forks(t_philo *p, int left, int right);
int		eat(t_philo *philo);
int		sleep_and_think(t_philo *philo);
void	release_forks(t_data *d, int left, int right);
void	inc_full(t_philo *p);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
long	get_timestamp(void);
int		init_philosophers(t_data *data, t_philo *philos);
int		init_all_mutexes(t_data *data, t_philo *philos);
int		init_data(t_data *data, t_philo **philos);

#endif