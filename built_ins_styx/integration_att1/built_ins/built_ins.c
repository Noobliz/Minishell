/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:41:11 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 19:41:16 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//echo's -n check for the newlines print
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
	if (str[i] = '\0')
		return (0);
	return (1);
}

//echo prints everything it has, with or without new_line dependent on above
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

//prints the pwd variable
//(later will show the hidden var)
int	pwd(t_env *env)
{
	char	*str;

	str = get_env("PWD", env);
	if (!str)
		return (-1);
	print(str);
	print("\n");
	return (0);
}

//here the function to call,
//it distributes over to different built_ins
//returns their error code (-1 malloc error, 0 success)
int	built_in_att1(int func, char **argv, char **envp, t_env *env)
{
	if (envp && !env)
	{
		env = create_env(envp);
		if (!env)
			return (-1);
	}
	if (func == 0)
		return (echo(argv));
	if (func == 1)
		return (pwd(env));
	if (func == 2)
		return (exportt(argv, env));
	if (func == 3)
		return (unset(argv, env));
	if (func == 4)
		disp_env(env);
	return (0);
}
