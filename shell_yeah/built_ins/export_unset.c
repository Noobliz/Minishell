/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:47:43 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 19:47:45 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//export syntax error
static void	print_exp_syntax(char *str)
{
	print("\033[35myou shell not pass:\033[0m : export: `");
	print(str);
	print("': not a valid identifier\n");
}

//export but the line by line process
static int	one_line_export(char **argv, int i, t_env *env)
{
	int	j;

	if (!argv[i])
		return (0);
	if (!is_alpha(argv[i][0]))
	{
		print_exp_syntax(argv[i]);
		return (one_line_export(argv, i + 1, env));
	}
	j = 1;
	while (argv[i][j] && argv[i][j] != '=')
	{
		if (!is_alphanum(argv[i][j])
			&& !(argv[i][j] == '+' && argv[i][j +1] == '='))
		{
			print_exp_syntax(argv[i]);
			return (one_line_export(argv, i + 1, env));
		}
		j++;
	}
	if (argv[i][j] == '=' && env
		&& add_env(env, argv[i]) == -1)
		return (-1);
	return (one_line_export(argv, i + 1, env));
}

//export, sets new variable unless syntax is poor
//if bad syntax in which case prints err message
int	exportt(char **argv, t_env *env)
{
	if (!argv)
		return (0);
	if (!argv[1])
		disp_env(env, 1);
	return (one_line_export(argv, 1, env));
}

//checking validity before sending it to delete
static int	check_var(char *var)
{
	int	i;

	i = 1;
	if (!is_alpha(var[0]))
		return (0);
	while (var[i])
	{
		if (!is_alphanum(var[i]))
			return (0);
		i++;
	}
	return (1);
}

//unsets var
int	unset(char **argv, t_env *env)
{
	int	i;

	if (!env)
		return (0);
	i = 1;
	while (argv[i])
	{
		if (check_var(argv[i]))
			del_env(env, argv[i]);
		i++;
	}
	return (0);
}
