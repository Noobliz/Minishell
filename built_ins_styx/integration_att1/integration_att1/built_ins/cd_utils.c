/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:26:19 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 12:29:09 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

char	*get_env_value(t_env *env, char *name)
{
	size_t	len;

	len = len_str(name);
	while (env)
	{
		if (ft_strncmp(env->var, name, len) == 0 && env->var[len] == '=')
			return (env->var + len + 1);
		env = env->next;
	}
	return (NULL);
}

char	*create_env_string(char *name, char *value)
{
	char	*new_var;
	size_t	len;

	len = len_str(name);
	new_var = malloc(len + len_str(value) + 2);
	if (!new_var)
		return (NULL);
	ft_strcpy(new_var, name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	return (new_var);
}

int	replace_if_exists(t_env *env, char *name, char *new_var)
{
	size_t	len;

	len = len_str(name);
	while (env)
	{
		if (ft_strncmp(env->var, name, len) == 0 && env->var[len] == '=')
		{
			free(env->var);
			env->var = new_var;
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	add_to_env(t_env **env, char *new_var)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	new->var = new_var;
	new->next = NULL;
	if (!*env)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}
