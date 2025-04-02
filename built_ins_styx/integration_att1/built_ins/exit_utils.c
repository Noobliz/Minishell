
#include "../libbig.h"

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	is_numeric(const char *str)
{
	int i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return;
	while (*s)
		write(fd, s++, 1);
}

void	ft_put3str_fd(const char *s1, const char *s2, const char *s3, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putstr_fd(s2, fd);
	ft_putstr_fd(s3, fd);
}