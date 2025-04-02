

#include <unistd.h>
#include <limits.h>
#include "../libbig.h"


int	check_sign(const char *str, int *i, int *overflow)
{
	int	sign;
	
	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		i++;
	}
	if (!str[*i])
		*overflow = 1;
	return (sign);
}

int	safe_atoi(const char *str, int *overflow)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	result = 0;
	*overflow = 0;
	sign = check_sign(str, &i, overflow);

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

void	free_exit_code(t_data *data, long long exit_code, char *str)
{
	if (str)
		ft_putstr_fd(str, 2);
	free_all_things(data);
	data->last_exit_code = exit_code;
	exit(exit_code);
}

int	ft_exit(t_data *data)
{
	int			overflow;
	long long	code;

	if (!data->cmds->argv[1])
	{
		ft_putstr_fd("exit\n", 2);
		free_exit_code(data, 0, "exit\n");
	}
	if (!is_numeric(data->cmds->argv[1]))
	{
		ft_put3str_fd("minishell: exit: ", data->cmds->argv[1], ": numeric argument required\n", 2);
		free_exit_code(data, 2, NULL);
	}
	code = safe_atoi(data->cmds->argv[1], &overflow);
	if (overflow)
		free_exit_code(data, 2, "exit\n");
	if (data->cmds->argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->last_exit_code = 1;
		return (1);
	}
	free_exit_code(data, code, "exit\n");
	return (0);
}


