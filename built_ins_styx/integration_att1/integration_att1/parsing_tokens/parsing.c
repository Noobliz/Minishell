/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:33:00 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/03 12:57:12 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//adjusts the tokens to take the single quotes into account
static int	handle_sgquotes(t_token *current)
{
	int	i;

	i = get_quote(current->value, '\'');
	if (split_token(current, i) == -1)
		return (-1);
	if (i)
		current = current->next;
	current->type = IGNORE;
	i = get_quote(current->value, '\'');
	if (i != -1)
	{
		if (i == 0)
			return (empty_quote(current));
		if (split_token(current, i) == -1)
			return (-1);
		return (0);
	}
	return (missing_quote('\''));
}

//adjusts the tokens to take the double quotes into account
static int	handle_dbquotes(t_token *current, t_env *env, char *lec)
{
	int	i;

	i = get_quote(current->value, '\"');
	if (split_token(current, i) == -1)
		return (-1);
	if (i)
		current = current->next;
	current->type = IGNORE;
	i = get_quote(current->value, '\"');
	if (i != -1)
	{
		if (i == 0)
			return (empty_quote(current));
		if (split_token(current, i) == -1)
			return (-1);
		if (handle_var(current, env, -1, lec) == -1)
			return (-1);
		return (0);
	}
	return (missing_quote('\"'));
}

//distribution station :: var or quotes
static int	distribute_parsing(t_token *current, int i, t_env *env, char *lec)
{
	if (current->value[i] == '\'')
		return (handle_sgquotes(current));
	if (current->value[i] == '\"')
		return (handle_dbquotes(current, env, lec));
	if (current->value[i] == '$')
		return (handle_var(current, env, i, lec));
	return (0);
}

//parsing per token
static int	parsing_inloop(t_token *current, t_env *env, char *lec)
{
	int	i;
	int	res;

	i = 0;
	while (current->value[i])
	{
		if (current->value[i] == '\'' || current->value[i] == '\"'
			|| current->value[i] == '$')
		{
			res = distribute_parsing(current, i, env, lec);
			if (res == -1 || res == -2)
				return (res);
			if (current->value[i] == '\'' || current->value[i] == '\"')
			{
				if (i)
					current = current->next;
				break ;
			}
			if (current->value[i] != '$')
				i--;
		}
		i++;
	}
	return (0);
}

//here, and then in double / handle_var and then same switch 
//in heredoc. take in char * already created, 
//use it like a get_env would return;
//full quotes and vars parsing
int	parsing_pt1(t_token *tokens, t_env *env, char *lec)
{
	t_token	*current;
	int		res;

	current = tokens;
	while (current)
	{
		res = parsing_inloop(current, env, lec);
		if (res == -1 || res == -2)
			return (res);
		current = current->next;
	}
	return (0);
}
