/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:54:02 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 19:54:05 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

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

//cree le nouvel environnement a partir de celui donne
t_env	*create_env(char **env)
{
	t_env	*head;
	t_env	*current;
	int		i;

	if (!env)
		return (NULL);
	head = env_item(env[0], 1);
	if (!head)
		return (NULL);
	i = 1;
	current = head;
	while (env[i])
	{
		current->next = env_item(env[i], 1);
		if (!current->next)
			return (free_env(head));
		current = current->next;
		i++;
	}
	return (head);
}
