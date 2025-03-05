/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:27:31 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/04 18:48:34 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libenv.h"

//hasty isalphanum
int	is_alphanum(char a)
{
	if ((a >= 'a' && a <= 'z')
		|| (a <= 'Z' && a >= 'A')
		|| (a <= '9' && a >= '0'))
		return (1);
	return (0);
}
//parcourt la variable environnement to_parse et cherche la variable donnee to_find
int	found(char *to_parse, char *to_find)
{
	int	i;

	i = 0;
	while (to_parse[i] != '=' && is_alphanum(to_find[i]))
	{
		if (to_parse[i] != to_find[i])
			return (0);
		i++;
	}
	if (to_parse[i] == '=' && !is_alphanum(to_find[i]))
		return (1);
	return (0);
}
//copy/strdup
char	*copy(char *str)
{
	char	*s;
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
		i++;
	s = malloc(i + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (str[i])
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
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
//cree le nouvel environnement a partir de celui donne
t_env	*create_env(char **env)
{
	t_env	*head;
	t_env	*current;
	int	i;

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
//pour ajouter une variable. prend la nouvelle chaine str (deja malloc et gardee, ne pas free) :: retourne 0 succes, -1 erreur de malloc
int	add_env(t_env *env, char *newvar)
{
	while (env->next)
		env = env->next;
	env->next = env_item(newvar, 0);
	if (!env->next)
		return (-1);
	return (0);
}
//pour supprimer une variable. prend le nom de la variable (VAR), et la supprime
void	del_env(t_env *env, char *del)
{
	t_env	*tmp;
	
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
//recupere une variable donnee (var donnee au debut du nom de la variable, par example $VAR sur le V), renvoie la valeur associee (dans VAR=VALUE, VALUE)
char	*get_env(char *var, t_env *env)
{
	t_env	*current;
	int	i;

	if (!var[0] || !is_alphanum(var[0]))
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
