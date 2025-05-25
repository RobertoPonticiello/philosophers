#include "philo.h"

long	get_timestamp(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	safe_print(t_philo *philo, const char *msg)
{
	long	ts;

	ts = get_timestamp() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_mutex);
	putnbr_fd(ts, 1);
	write(1, " ", 1);
	putnbr_fd(philo->id, 1);
	write(1, " ", 1);
	write(1, msg, str_len(msg));
	write(1, "\n", 1);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

