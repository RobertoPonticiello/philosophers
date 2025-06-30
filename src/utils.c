#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	r;

	r = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		r = r * 10 + *str - '0';
		str++;
	}
	return (sign * r);
}

void	putnbr_fd(long n, int fd)
{
	char	c;

	if (n >= 10)
		putnbr_fd(n / 10, fd);
	c = '0' + (n % 10);
	write(fd, &c, 1);
}

size_t	str_len(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
