#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data
{
	int				n_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	long			start_time;
}	t_data;

typedef struct s_philo
{
	int			id;
	long		last_meal;
	t_data		*data;
	pthread_t	thread;
}	t_philo;

int	ft_atoi(const char *str);
void putnbr_fd(long n, int fd);
size_t str_len(const char *s);
int	parse_args(int argc, char **argv, t_data *data);
int	init_data(t_data *data, t_philo **philos);
void *philosopher_routine(void *arg);
long	get_timestamp(void);
void	safe_print(t_philo *philo, const char *msg);
int	init_mutexes(t_data *data);
void	destroy_mutexes(t_data *data);





#endif