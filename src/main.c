#include "philo.h"

int main(int argc, char **argv)
{
	t_data    data; t_philo *philos; pthread_t *threads, monitor; int i;
	if (parse_args(argc, argv, &data) || init_data(&data, &philos))
		return (1);
	threads = malloc(sizeof(pthread_t) * data.n_philo);
	if (!threads)
		return (1);
	i = 0;
	while (i < data.n_philo)
		pthread_create(&threads[i], NULL, philosopher_routine, &philos[i]), i++;
	pthread_create(&monitor, NULL, monitor_routine, &data);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data.n_philo)
		pthread_join(threads[i], NULL), i++;
	destroy_mutexes(&data);
	free(philos);
	free(threads);
	return (0);
}