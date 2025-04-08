/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:26:19 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/08 17:22:26 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

char	*ft_strdup(char *s)
{
	char	*dup;
	int		i;

	if (!s)
		return (NULL);
	i = len_str(s);
	dup = malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*get_env_value(t_env *env, char *name)
{
	size_t	len;
	t_env	*tmp;

	len = len_str(name);
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->var, name, len) == 0 && tmp->var[len] == '=')
			return (tmp->var + len + 1);
		tmp = tmp->next;
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
