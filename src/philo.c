#include "philo.h"

int parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		write (2, "Usage: ./philo n_philo time_to_die time_to_eat "
			"time_to_sleep [must_eat_count]\n",
			81);
		return (1);
	}
	data->n_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	if (data->n_philo <= 0 || data->time_to_die <= 0 ||
		data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
		(argc == 6 && data->must_eat_count <= 0))
	{
		write(2, "All arguments must be positive\n", 44);
		return(1);
	}
	return (0);
}
/*
void safe_print(t_philo *p, char *msg)
{
	pthread_mutex_lock(&p->data->print_mutex);
	printf("%ld %d %s\n",
		get_timestamp() - p->data->start_time,
		p->id, msg);
	pthread_mutex_unlock(&p->data->print_mutex);
}

long get_timestamp(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

while (1) {
	for each p in philos
	{
		if (get_timestamp() - p->last_meal > data->time_to_die) {
			safe_print(p, "died");
			exit_program();
		}
	}
}
*/