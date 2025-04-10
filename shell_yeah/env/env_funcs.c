/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:27:31 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 14:54:03 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

void	disp_env(t_env *env, int exp)
{
	while (env)
	{
		if (is_alpha(env->var[0]))
		{
			if (exp)
				print("declare -x ");
			print(env->var);
			print("\n");
		}
		env = env->next;
	}
}

//update_var, in case of an existing variable to be changed
int	update_var(t_env *env, char *newvar)
{
	char	*var;
	char	*joined;

	while (!found(env->var, newvar))
		env = env->next;
	var = take_out_plus(newvar, 1);
	if (!var)
		return (-1);
	if (len_str(var) == len_str(newvar))
	{
		free(env->var);
		env->var = var;
	}
	else
	{
		joined = join(env->var, var);
		free(var);
		if (!joined)
			return (-1);
		free(env->var);
		env->var = joined;
	}
	return (0);
}

int	add_env(t_env *env, char *newvar)
{
	char	*var;

	var = get_env(newvar, env);
	if (var)
		return (update_var(env, newvar));
	var = take_out_plus(newvar, 0);
	if (!var)
		return (-1);
	while (env->next)
		env = env->next;
	env->next = env_item(var, 0);
	if (!env->next)
		return (-1);
	return (0);
}

void	del_env(t_env *env, char *del)
{
	t_env	*tmp;

	if (!del || !is_alpha(del[0]))
		return ;
	while (env->next)
	{
		if (found(env->next->var, del))
			break ;
		env = env->next;
	}
	if (!env->next)
		return ;
	tmp = env->next;
	env->next = env->next->next;
	free(tmp->var);
	free(tmp);
}

char	*get_env(char *var, t_env *env)
{
	t_env	*current;
	int		i;

	if (!var[0])
		return ("$");
	current = env;
	i = 0;
	while (current)
	{
		if (found(current->var, var))
		{
			while (current->var[i] != '=')
				i++;
			return (&current->var[i + 1]);
		}
		current = current->next;
	}
	return (NULL);
}
