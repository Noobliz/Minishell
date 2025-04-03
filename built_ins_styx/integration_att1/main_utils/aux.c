/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:49:21 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 12:52:33 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//checks if the str cat and copy are the same
int	isis(char *cat, char *copy)
{
	int	i;

	i = 0;
	if (!cat || !copy)
		return (0);
	while (cat[i] && cat[i] == copy[i])
		i++;
	if (cat[i] != copy[i])
		return (0);
	return (1);
}

int	free_all_things(t_data *data)
{
	if (data->env_array)
	{
		free(data->env_array);
		data->env_array = NULL;
	}
	if (data->env)
		free_env(data->env);
	if (data->token)
	{
		free_tokens(data->token);
		data->token = NULL;
	}
	if (data->cmds)
	{
		free_cmds_new(data->cmds, data->cmds->next);
		data->cmds = NULL;
	}
	if (data->prompt)
	{
		free(data->prompt);
		data->prompt = NULL;
	}
	rl_clear_history();
	return (0);
}

//hasty itoa (for the err_code)
char	*num_str(int code)
{
	char	*lec;
	int		len;
	int		tens;

	len = 0;
	tens = 1;
	while (code / tens > 0)
	{
		len++;
		tens = tens * 10;
	}
	if (code == 0)
		len = 1;
	lec = malloc(len + 1);
	if (!lec)
		return (NULL);
	lec[len] = '\0';
	while (len > 0)
	{
		lec[len -1] = code % 10 + '0';
		len--;
		code = code / 10;
	}
	return (lec);
}

static int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		if (env->var)
			size++;
		env = env->next;
	}
	return (size);
}

// transform the t_env to char ** for execve
char	**env_to_array(t_env *env)
{
	int		i;
	char	**env_array;
	int		size;

	size = env_size(env);
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (perror("malloc"), NULL);
	i = 0;
	while (env)
	{
		if (env->var)
		{
			env_array[i] = env->var;
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
