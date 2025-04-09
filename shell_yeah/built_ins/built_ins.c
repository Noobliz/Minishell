/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:41:11 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/09 13:05:41 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

// echo's -n check for the newlines print
int	check_n(char *str)
{
	int	i;

	if (!str)
		return (0);
	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

// prints everything it has
// with or without new_line dependent on above
int	echo(char **argv)
{
	int	new_line;
	int	i;

	i = 1;
	new_line = 1;
	if (!argv)
		return (0);
	while (check_n(argv[i]))
	{
		new_line = 0;
		i++;
	}
	while (argv[i])
	{
		print(argv[i]);
		i++;
		if (argv[i])
			print(" ");
	}
	if (new_line)
		print("\n");
	return (0);
}

// prints the 1pwd var
int	pwd(t_env *env)
{
	char	*str;

	str = get_env("1PWD", env);
	if (!str)
		return (1);
	print(str);
	print("\n");
	return (0);
}

// distributes over to different built_ins
// returns their error code
int	built_in(int func, char **argv, t_data *data, int fork)
{
	if (!data->env)
		return (12);
	if (func == 0)
		return (echo(argv));
	if (func == 1)
		return (pwd(data->env));
	if (func == 2)
		return (exportt(argv, data->env));
	if (func == 3)
		return (unset(argv, data->env));
	if (func == 4)
		disp_env(data->env, 0);
	if (func == 5)
		return (ft_exit(argv, data, fork));
	if (func == 6)
		return (cd(argv, &data->env));
	return (0);
}
