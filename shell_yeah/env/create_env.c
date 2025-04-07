/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:54:02 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/03 12:33:45 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

int	update_lst_cmd(t_env *env, t_cmd *cmd)
{
	char	*res;
	int		here;

	if (!cmd)
		return (0);
	while (cmd->next)
		cmd = cmd->next;
	while (cmd)
	{
		if (cmd->argv)
			break ;
		cmd = cmd->previous;
	}
	if (!cmd)
		return (0);
	res = join("_=", cmd->argv[0]);
	if (!res)
		return (-1);
	del_env(env, "_");
	here = add_env(env, res);
	free(res);
	return (here);
}

//cree une nouvelle structure env et la renvoie
t_env	*env_item(char *var, int cpy)
{
	t_env	*item;

	item = (t_env *)malloc(sizeof(t_env));
	if (!item)
		return (NULL);
	if (cpy)
		item->var = copy(var);
	else
		item->var = var;
	if (!item->var)
	{
		free(item);
		return (NULL);
	}
	item->next = NULL;
	return (item);
}

//free l'environnement, retourne NULL
t_env	*free_env(t_env *env)
{
	t_env	*res;

	if (!env)
		return (NULL);
	res = free_env(env->next);
	free(env->var);
	free(env);
	return (res);
}

static t_env	*get_inv_path(void)
{
	char	*path;
	char	*var;
	t_env	*res;

	path = getcwd(NULL, 0);
	if (!path)
		path = "";
	var = join("1PWD=", path);
	if (path[0])
		free(path);
	if (!var)
		return (NULL);
	res = env_item(var, 0);
	if (!res)
		return (NULL);
	return (res);
}

//cree le nouvel environnement a partir de celui donne
t_env	*create_env(char **env)
{
	t_env	*head;
	t_env	*current;
	int		i;

	if (!env)
		return (NULL);
	head = get_inv_path();
	if (!head)
		return (NULL);
	i = 0;
	current = head;
	while (env && env[i])
	{
		current->next = env_item(env[i], 1);
		if (!current->next)
			return (free_env(head));
		current = current->next;
		i++;
	}
	return (head);
}
