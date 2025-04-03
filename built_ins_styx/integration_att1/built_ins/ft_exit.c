/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:30:43 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 15:24:11 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		ft_putstr_fd(str, 1);
	free_all_things(data);
	data->last_exit_code = exit_code;
	exit(exit_code);
}

int	ft_exit(char **argv, t_data *data)
{
	int			overflow;
	long long	code;

	if (argv[1] == NULL)
		free_exit_code(data, 0, "exit\n");
	if (!is_numeric(argv[1]))
	{
		ft_put3str_fd("minishell: exit: ", data->cmds->argv[1],
			": numeric argument required\n", 2);
		free_exit_code(data, 2, NULL);
	}
	code = safe_atoi(argv[1], &overflow);
	if (overflow)
		free_exit_code(data, 2, "exit\n");
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->last_exit_code = 1;
		return (1);
	}
	free_exit_code(data, code, "exit\n");
	return (1);
}
