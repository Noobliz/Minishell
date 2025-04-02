/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:41:11 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/02 16:17:08 by lisux            ###   ########.fr       */
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

// echo prints everything it has, with or without new_line dependent on above
int	echo(char **argv)
{
	int	new_line;
	int	i;

	i = 1;
	new_line = 1;
	if (!argv)
		return (0);
	if (check_n(argv[1]))
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

// prints the pwd variable
//(later will show the hidden var)
int	pwd(t_env *env)
{
	char	*str;

	str = get_env("1PWD", env);
	if (!str)
		return (-1);
	print(str);
	print("\n");
	return (0);
}

// here the function to call,
// it distributes over to different built_ins
// returns their error code (-1 malloc error, 0 success)
int	built_in(int func, char **argv, char **envp, t_data *data)
{
	if (envp && !data->env)
	{
		data->env = create_env(envp);
		if (!data->env)
			return (-1);
	}
	if (func == 0)
		return (echo(argv));
	if (func == 1)
		return (pwd(data->env));
	if (func == 2)
		return (exportt(argv, data->env));
	if (func == 3)
		return (unset(argv, data->env));
	if (func == 4)
		disp_env(data->env);
	if (func == 5)
		ft_exit(data);
	if (func == 6)
		cd(argv, &data->env);
	return (0);
}
