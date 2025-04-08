/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:47:43 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 17:23:47 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	print_exp_syntax(char *str)
{
	print_err("\033[35myou shell not pass:\033[0m : export: `");
	print_err(str);
	print_err("': not a valid identifier\n");
	return (1);
}

static int	one_line_export(char **argv, int i, t_env *env, int res)
{
	int	j;

	if (!argv[i])
		return (res);
	if (!is_alpha(argv[i][0]))
	{
		res = print_exp_syntax(argv[i]);
		return (one_line_export(argv, i + 1, env, res));
	}
	j = 1;
	while (argv[i][j] && argv[i][j] != '=')
	{
		if (!is_alphanum(argv[i][j])
			&& !(argv[i][j] == '+' && argv[i][j +1] == '='))
		{
			res = print_exp_syntax(argv[i]);
			return (one_line_export(argv, i + 1, env, res));
		}
		j++;
	}
	if (argv[i][j] == '=' && env
		&& add_env(env, argv[i]) == -1)
		return (12);
	return (one_line_export(argv, i + 1, env, res));
}

//sets new variable unless syntax is poor
//if bad syntax prints err message
int	exportt(char **argv, t_env *env)
{
	if (!argv)
		return (0);
	if (!argv[1])
		disp_env(env, 1);
	return (one_line_export(argv, 1, env, 0));
}

//checking validity before sending it to del_env
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
