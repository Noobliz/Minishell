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

#include ""
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
//recupere une variable donnee (var donnee au debut du nom de la variable, par example $VAR sur le V)
char	*get_env(char *var, t_env *env)
{
	t_env	*current;
	int	i;

	if (!var[0])
		return (NULL);
	if (!is_alphanum(var[0]))
		return ("$");
	current = env;
	i = 0;
	while (current)
	{
		if (found(current, var))
		{
			while (current[i] != '=')
				i++;
			return (current[i + 1]);
		}
		current = current->next;
	}
	return (NULL);
}
char	*replace(char *s, )
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
		var = replace(token->value, "", i + 1);
	else
		var = replace(token->value, var, i + 1);
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
	int	end;

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
	int	i;
	char	*str;
	char	**splat;
	t_token	*item;

	i = 0;
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
	item = new_token(splat[1], token->type, token);
	free(token->value);
	token->value = splat[0];
	str = splat[1];
	free(splat);
	if (!item)
	{
		free(str);
		return (-1);
	}
	item->next = token->next;
	token->next->back = item;
	token->next = item;
	return (0);
}
//joins two tokens into one
int	add_up(t_token *beg, t_token *end)
{
	char	*str;

	str = join(beg->value, end->value);
	if (!str)
		return (-1);
	free(beg->value);
	free(end->value);
	beg->next = end->next;
	end->next->back = beg;
	free(end);
	return (0);
}
//adjusts the tokens to take the single quotes into account, starting from the first quote found
int	handle_sgquotes(t_token *current)
{
	int	i;
	t_token	*end;

	i = get_quote(current->value, '\'');
	if (split_token(current, i) == -1)
		return (-1);
	i = get_quote(current->value, '\'');
	if (i != -1)
	{
		if (split_token(current, i) == -1)
			return (-1);
		return (0);
	}
	end = current;
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
	if (!end)
		return (-1);
	return (0);
}

int	handle_dbquotes(t_token *current, t_env *env)
{
	int	i;
	char	*str;
	t_token	*end;

	i = get_quote(current->value, '\"');
	if (split_token(current, i) == -1)
		return (-1);
	i = get_quote(current->value, '\"');
	if (i != -1)
	{
		if (split_token(current, i) == -1)
			return (-1);
		return (0);
	}
	end = current;
	while (end)
	{
		i = get_quote(end->value, '\'');
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
	if (!end)
		return (-1);
	return (0);
	
//le parsing !!
int	parsing_pt_2(t_token *tokens, t_env *env)
{
	t_token	*current;
	int	i;

	current = tokens;
	while (current)
	{
		i = 0;
		while (current->value[i])
		{
			if (current->value[i] == '\'')
			{
				if (handle_sgquotes(current) == -1)
					return (-1);
				break ;
			}
			if (current->value[i] == '\"')
			{
				if (handle_dbquotes(current, env) == -1)
					return (-1);
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
