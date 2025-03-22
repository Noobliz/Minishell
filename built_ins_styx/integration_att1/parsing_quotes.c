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

#include "libbig.h"

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
//getting the last character of a string
char	last_char(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return ('\0');
	while (str[i])
		i++;
	return (str[i - 1]);
}
//splits all inner spaces of a token
//maybe we can make every add_up opportunity into a HEREDOC type ? that way, no confusion, all ignores stick together; that's it;
int	split_inner_spaces(t_token *token)
{
	int	sign = 0;
	int	space;
	t_token *tmp = token;
	char	*var;

	if (token->value[0] != ' ')
		sign = 1;
	if (last_char(token->value) != ' ')
		sign = sign + 2;
	space = get_quote(token->value, ' ');
	while (space != -1)
	{
		var = del_spaces(token->value);
		if (!var)
			return (-1);
		free(token->value);
		token->value = var;
		space = get_quote(token->value, ' ');
		if (space != -1 && split_token(token, space) == -1)
			return (-1);
		if (space != -1)
			token = token->next;
	}
	if ((sign == 1 || sign == 3)
		&& tmp->previous && tmp->previous->type == IGNORE)
		tmp->type = HEREDOC;
	if ((sign == 2 || sign == 3)
		&& token->next && token->next->type == IGNORE)
		token->type = HEREDOC;
	return (0);
}
void	delete_token(t_token *token)
{
	if (!token)
		return ;
	if (token->previous)
		token->previous->next = token->next;
	if (token->next)
		token->next->previous = token->previous;
	if (token->value)
		free(token->value);
	free(token);
}
//resets all tokens to CMD type to avoid later confusion;
void	all_cmd_type(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	while (token->previous)
		token = token->previous;
	while (token)
	{
		tmp = token->next;
		if (token->type != IGNORE && token->value && !token->value[0])
			delete_token(token);
		else if (token->type != IGNORE)
			token->type = CMD;
		token = tmp;
	}
}
//first we trim and split the non quotes tokens, turning them into HEREDOC should they be added with the next or previous IGNORE token;
int	preface_quotes(t_token *token)
{
	t_token	*tmp = token;

	while (token)
	{
		tmp = token->next;
		if (token->type != IGNORE && token->type != HEREDOC)
		{
			if (split_inner_spaces(token) == -1)
				return (-1);
		}
		token = tmp;
	}
	return (0);
}
//fix_quotes is our new trim_split, it also adjusts the quotese depending on whether or not they're supposed to be part of a bigger token;
int	fix_quotes(t_token *token)
{
	if (preface_quotes(token) == -1)
		return (-1);
	while (token)
	{
		if (token->type == IGNORE && token->next)
		{
			if (token->next->type == IGNORE || token->next->type == HEREDOC)
			{
				if (add_up(token, token->next) == -1)
					return (-1);
			}
		}
		else if (token->type == HEREDOC && token->next)
		{
			if (token->next->type == IGNORE)
			{
				if (add_up(token, token->next) == -1)
					return (-1);
			}
		}
		token = token->next;
	}
	return (0);
}
//trimming tokens (if type != IGNORE, delete surrounding spaces (ex: "     cmd   " becomes "cmd"))
//splitting tokens (if type != IGNORE, splits tokens according to spaces between them (ex: "cmd    opt" becomes "cmd", "opt")) -- unused now, switched to thhe fix_quotes func;
int	trim_split_tokens(t_token *token)
{
	char 	*value;
	int	where;

	if (fix_quotes(token) == -1)
		return (-1);
	while (token)
	{
		if (token->type != IGNORE)
		{
			value = del_spaces(token->value);
			if (!value)
				return (-1);
			free(token->value);
			token->value = value;
			where = get_quote(token->value, ' ');
			if (where != -1 && split_token(token, where) == -1)
				return (-1);
		}
		token = token->next;
	}
	return (0);
}
//ok so this one is supposed to take in the string (token->value), the variable (from get_env), and the index of the $, and it returns that same string with the variable substituted (ie for VAR=VALUE we go from "here $VAR" to "here VALUE")
//!!it does not free the inital strings given, for malloc protection reasons
char	*replace(char *s, char *var, int where, int next)
{
	int	i;
	int	len = 0;
	char	*newstr;

	while (s[next + len])
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
	while (s[next])
	{
		newstr[len] = s[next];
		len++;
		next++;
	}
	newstr[len] = '\0';
	return (newstr);
}
//for the variables given as :: ${VAR}
int	handle_acc_var(t_token *token, t_env *env, int i)
{
	int	j;
	char	*var;

	j = i;
	while (token->value[j] && token->value[j] != '}')
		j++;
	if (!token->value[j])
		return (missing_quote('}'));
	var = get_env(&token->value[i + 2], env);
	if (!var)
		var = replace(token->value, "", i, j + 1);
	else
		var = replace(token->value, var, i, j + 1);
	if (!var)
		return (-1);
	free(token->value);
	token->value = var;
	return (0);
}
//for the variables given like this :: $VAR
int	handle_reg_var(t_token *token, t_env *env, int i)
{
	int	j;
	char	*var;

	j = i + 1;
	while (token->value[j] && is_alphanum(token->value[j]))
		j++;
	var = get_env(&token->value[i + 1], env);
	if (!var)
		var = replace(token->value, "", i, j);
	else
		var = replace(token->value, var, i, j);
	if (!var)
		return (-1);
	free(token->value);
	token->value = var;
	return (0);
}
//celle-ci elle prend en compte si tu veux faire juste la premiere variable rencontree (here != -1)ou toutes les variables (here == -1), et elle remplace le $VAR par sa valeur dans la *str token->value
int	handle_var(t_token *token, t_env *env, int here)
{
	int	i;

	i = 0;
	while (token->value[i] && !(token->value[i] == '$'
		&& (is_alpha(token->value[i + 1]) || token->value[i + 1] == '{')))
		i++;
	if (!token->value[i])
		return (0);
	if (token->value[i + 1] == '{')
	{
		i = handle_acc_var(token, env, i);
		if (i == -2 || here > -1)
			return (i);
		else
			return (handle_var(token, env, here));
	}
	if (handle_reg_var(token, env, i) == -1)
		return (-1);
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
//splits the token at the quote index into two separate tokens (quote excluded of course)
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

	str = join(beg->value, end->value);
	if (!str)
		return (-1);
	free(beg->value);
	free(end->value);
	beg->value = str;
	beg->next = end->next;
	if (end->next)
		end->next->previous = beg;
	free(end);
	return (0);
}
//in case of empty quotes :: "" or ''
int	empty_quote(t_token *token)
{
	char	*str;

	str = copy(&token->value[1]);
	if (!str)
		return (-1);
	free(token->value);
	token->value = copy("");
	if (!token->value)
	{
		free(str);
		return (-1);
	}
	token->next = new_token(str, CMD, token);
	if (!token->next)
	{
		free(str);
		return (-1);
	}
	return (0);
}
//adjusts the tokens to take the single quotes into account, starting from the first quote found
int	handle_sgquotes(t_token *current)
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
//adjusts the tokens to take the double quotes into account, starting from the first quote found
int	handle_dbquotes(t_token *current, t_env *env)
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
		if (handle_var(current, env, -1) == -1)
			return (-1);
		return (0);
	}
	return (missing_quote('\"'));
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
				if (res == -1 || res == -2)
					return (res);
				if (i)
					current = current->next;
				break ;
			}
			if (current->value[i] == '\"')
			{
				res = handle_dbquotes(current, env);
				if (res == -1 || res == -2)
					return (res);
				if (i)
					current = current->next;
				break ;
			}
			if (current->value[i] == '$')
			{
				if (handle_var(current, env, i) == -1)
					return (-1);
				i--;
			}
			i++;
		}
		current = current->next;
	}
	return (0);
}

void	free_tokens(t_token *token)
{
	if (!token)
		return ;
	free_tokens(token->next);
	if (token->value)
		free(token->value);
	free(token);
}
