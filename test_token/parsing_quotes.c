/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:33:00 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/03 18:10:52 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libparsing.h"
#include <stdio.h>
//getting rid of spaces in the appropriate tokens here (token->type == CMD)
char	*del_spaces(char *str)
{
	char	*s;
	int	i = 0;
	int	j;
	int	len;

	while (str[i] == ' ')
		i++;
	j = i;
	while (str[i + 1])
		i++;
	while (i && str[i] == ' ')
	{
		i--;
		j++;
	}
	len = len_str(str) - j;
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] == ' ')
		i++;
	while (j < len)
	{
		s[j] = str[i + j];
		j++;
	}
	s[j] ='\0';
	return (s);
}
//trimming tokens (if type == CMD, delete surrounding spaces (ex: "     cmd   " becomes "cmd"))
int	trim_tokens(t_token *token)
{
	char 	*value;

	while (token)
	{
		if (token->type == CMD)
		{
			value = del_spaces(token->value);
			if (!value)
				return (-1);
			free(token->value);
			token->value = value;
		}
		token = token->next;
	}
	return (0);
}

int	len_str(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char  *join(char *s, char *s2)
{
  int len;
  int i;
  int j;
  char  *str;

  if (!s && !s2)
  	return (NULL);
  len = len_str(s);
  len = len + len_str(s2);
  str = malloc(len + 1);
  if (!str)
    return (NULL);
  i = 0;
  while (s && s[i])
  {
    str[i] = s[i];
    i++;
  }
  j = 0;
  while (s2 && s2[j])
  {
    str[i + j] = s2[j];
    j++;
  }
  str[i + j] = '\0';
  return (str);
}
//ok so this one is supposed to take in the string (token->value), the variable (from get_env), and the index of the $, and it returns that same string with the variable substituted (ie for VAR=VALUE we go from "here $VAR" to "here VALUE")
//!!it does not free the inital strings given, for malloc protection reasons
char	*replace(char *s, char *var, int where)
{
	int	i = 0;
	int	len = 0;
	char	*newstr;

	while (i <= where || is_alphanum(s[i]))
		i++;
	while (s[i + len])
		len++;
	len = len + where + len_str(var);
	newstr = malloc(len + 1);
	if (!newstr)
		return (NULL);
	i = 0;
	while (i < where)
	{
		newstr[i] = s[i];
		i++;
	}
	len = 0;
	while (var[len])
	{
		newstr[i + len] = var[len];
		len++;
	}
	len += i;
	while (i == where || is_alphanum(s[i]))
		i++;
	while (s[i])
	{
		newstr[len] = s[i];
		len++;
		i++;
	}
	newstr[len] = '\0';
	return (newstr);
}
//celle-ci elle prend en compte si tu veux faire juste la premiere variable rencontree (here != -1)ou toutes les variables (here == -1), et elle remplace le $VAR par sa valeur dans la *str token->value
int	handle_var(t_token *token, t_env *env, int here)
{
	int	i;
	char	*var;

	i = 0;
	while (token->value[i] && token->value[i] != '$')
		i++;
	if (!token->value[i])
		return (0);
	var = get_env(&token->value[i + 1], env);
	if (!var)
		var = replace(token->value, "", i);
	else
		var = replace(token->value, var, i);
	if (!var)
		return (-1);
	free(token->value);
	token->value = var;
	if (here == -1)
		return (handle_var(token, env, here));
	return (0);
}
//comme un split mais un seul, a l'index quote, au cas ou il y a plusieurs quotes dedans (quote exclue bein sur)
char	**split_once(char *str, int quote)
{
	char	**res;
	int	i;

	res = malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	res[0] = malloc(quote + 1);
	if (!res[0])
	{
		free(res);
		return (NULL);
	}
	i = 0;
	while (i < quote)
	{
		res[0][i] = str[i];
		i++;
	}
	res[0][i] = '\0';
	i = 0;
	while (str[i + 1 + quote])
		i++;
	res[1] = malloc(i + 1);
	if (!res[1])
	{
		free(res[0]);
		free(res);
		return (NULL);
	}
	i = 0;
	while (str[i + 1 + quote])
	{
		res[1][i] = str[i + 1 + quote];
		i++;
	}
	res[1][i] = '\0';
	res[2] = NULL;
	return (res);
}
		

//same as your create_tokens, except no strdup and i've added the previous token
t_token *new_token(char *value, t_type type, t_token *prev)
{
	t_token *new = malloc(sizeof(t_token));
	if (!new)
		return(NULL);
	new->value = value;
	new->type = type;
	new->next = NULL;
    	new->previous = prev;
	return (new);
}
//takes the value and finds the index of the quote, or -1 if no quote
int	get_quote(char *token, char quote)
{
	int	i;

	i = 0;
	while (token[i] && token[i] != quote)
		i++;
	if (token[i])
		return (i);
	return (-1);
}
//splits the token at the quote point into two separate tokens (quote excluded of course)
int	split_token(t_token *token, int	quote)
{
	char	*str;
	char	**splat;
	t_token	*item = NULL;

	if (quote == 0)
	{
		str = copy(&token->value[1]);
		if (!str)
			return (-1);
		free(token->value);
		token->value = str;
		return (0);
	}
	splat = split_once(token->value, quote);
	if (!splat)
		return (-1);
	if (splat[1][0])
		item = new_token(splat[1], CMD, token);
	free(token->value);
	token->value = splat[0];
	str = splat[1];
	free(splat);
	if (!item && str[0])
	{
		free(str);
		return (-1);
	}
	if (!item)
		free(str);
	else
		item->next = token->next;
	if (token->next)
		token->next->previous = item;
	token->next = item;
	return (0);
}
//joins two tokens into one --unused currently
int	add_up(t_token *beg, t_token *end)
{
	char	*str;
	char	*str2;

	str = join(beg->value, " ");
	if (!str)
		return (-1);
	str2 = join(str, end->value);
	free(str);
	if (!str2)
		return (-1);
	free(beg->value);
	free(end->value);
	beg->value = str2;
	beg->next = end->next;
	if (end->next)
		end->next->previous = beg;
	free(end);
	return (0);
}
//adjusts the tokens to take the single quotes into account, starting from the first quote found
int	handle_sgquotes(t_token *current)
{
	int	i;
	//t_token	*end;

	i = get_quote(current->value, '\'');
	if (split_token(current, i) == -1)
		return (-1);
	if (i)
	  current = current->next;
	current->type = IGNORE;
	i = get_quote(current->value, '\'');
	if (i != -1)
	{
		if (split_token(current, i) == -1)
			return (-1);
		return (0);
	}
	/*end = current->next;
	while (end)
	{
		i = get_quote(end->value, '\'');
		if (i != -1)
		{
			if (split_token(end, i) == -1)
				return (-1);
			if (add_up(current, current->next) == -1)
				return (-1);
			return (0);
		}
		if (add_up(current, end) == -1)
			return (-1);
		end = current->next;
	}
	if (!end)*/
	return (-2);
	//return (0);
}

int	handle_dbquotes(t_token *current, t_env *env)
{
	int	i;
	//t_token	*end;

	i = get_quote(current->value, '\"');
	if (split_token(current, i) == -1)
		return (-1);
	if (i)
	  current = current->next;
	current->type = IGNORE;
	i = get_quote(current->value, '\"');
	if (i != -1)
	{
		if (split_token(current, i) == -1)
			return (-1);
		if (handle_var(current, env, -1) == -1)
			return (-1);
		return (0);
	}
	/*end = current->next;
	while (end)
	{
		i = get_quote(end->value, '\"');
		if (i != -1)
		{
			if (split_token(end, i) == -1)
				return (-1);
			if (add_up(current, current->next) == -1)
				return (-1);
			if (handle_var(current, env, -1) == -1)
				return (-1);
			return (0);
		}
		if (add_up(current, end) == -1)
			return (-1);
		end = current->next;
	}
	if (!end)*/
	return (-2);
	//return (0);
}
	
//le parsing !!
int	parsing_pt_2(t_token *tokens, t_env *env)
{
	t_token	*current;
	int	i;
	int	res;

	current = tokens;
	while (current)
	{
		i = 0;
		while (current->value[i])
		{
			if (current->value[i] == '\'')
			{
				res = handle_sgquotes(current);
				if (res == -1)
					return (-1);
				if (res == -2)
					return (-2);
				if (i)
					current = current->next;
				break ;
			}
			if (current->value[i] == '\"')
			{
				res = handle_dbquotes(current, env);
				if (res == -1)
					return (-1);
				if (res == -2)
					return (-2);
				if (i)
					current = current->next;
				break ;
			}
			if (current->value[i] == '$')
				if (handle_var(current, env, i) == -1)
					return (-1);
			i++;
		}
		current = current->next;
	}
	return (0);
}
