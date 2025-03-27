

#include <unistd.h>
#include <limits.h>
#include "../libbig.h"


int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
static int	is_numeric(const char *str)
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

int	safe_atoi(const char *str, int *overflow)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	sign = 1;
	result = 0;
	*overflow = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		*overflow = 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if ((sign == 1 && result > LLONG_MAX)
			|| (sign == -1 && result > (unsigned long)LLONG_MAX + 1))
		{
			*overflow = 1;
			return (0);
		}
		i++;
	}
	if (str[i] != '\0')
		*overflow = 1;
	return ((int)(sign * result));
}

// int	ft_exit(char **argv, t_cmd *cmds)
// {
// 	int			overflow;
// 	long long	code;

// 	if (!argv[1])
// 	{
// 		ft_putstr_fd("exit\n", 2);
// 		//free_all
// 		exit(cmds->last_exit_code);
// 	}
// 	if (!is_numeric(argv[1]))
// 	{
// 		ft_put3str_fd("minishell: exit: ", argv[1], ": numeric argument required\n", 2);
// 		//free_all
// 		exit(2);
// 	}
// 	code = safe_atoi(argv[1], &overflow);
// 	if (overflow)
// 	{
// 		ft_putstr_fd("exit\n", 2);
// 		//free_all
// 		exit(2);
// 	}
// 	if (argv[2])
// 	{
// 		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
// 		return (1);
// 	}
// 	ft_putstr_fd("exit\n", 2);
// 	exit((unsigned char)code);
// }

int	ft_exit(t_data *data)
{
	char		**argv = data->cmds->argv;
	int			overflow;
	long long	code;

	if (!argv[1])
	{
		ft_putstr_fd("exit\n", 2);
		free_all_things(data);
		data->last_exit_code = 0;
		exit(0);
	}
	if (!is_numeric(argv[1]))
	{
		ft_put3str_fd("minishell: exit: ", argv[1], ": numeric argument required\n", 2);
		free_all_things(data);
		data->last_exit_code = 2;
		exit(2);
	}
	code = safe_atoi(argv[1], &overflow);
	if (overflow)
	{
		ft_putstr_fd("exit\n", 2);
		free_all_things(data);
		data->last_exit_code = 2;
		exit(2);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->last_exit_code = 1;
		return (1);
	}
	ft_putstr_fd("exit\n", 2);
	free_all_things(data);
	data->last_exit_code = ((unsigned char)code);
	exit((unsigned char)code);
}


