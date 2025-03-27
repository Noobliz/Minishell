/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:42:44 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 13:43:03 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//for HEREDOC and APPEND, gets rid of the second < or >
static int	double_types(int where, t_token *token)
{
	char	*value;

	if (where)
		token = token->next;
	if (!token->value)
		return (0);
	value = copy(&token->value[1]);
	if (!value)
		return (-1);
	free(token->value);
	token->value = value;
	return (0);
}

//gets the char associated with type
static char	get_a(int type)
{
	char	a;

	a = '0';
	if (type == PIPE)
		a = '|';
	if (type == REDIR_IN || type == HEREDOC)
		a = '<';
	if (type == REDIR_OUT || type == APPEND)
		a = '>';
	return (a);
}

static int	assess_types_token(t_token **token, t_token *tmp)
{
	if (!(*token)->previous)
	{
		(*token)->previous = tmp;
		tmp->next = *token;
		return (0);
	}
	*token = (*token)->previous;
	return (1);
}

//splits the token and adds the new type_token either in between or before it
int	split_type(t_token *token, int type)
{
	int		where;
	char	a;
	t_token	*tmp;

	a = get_a(type);
	where = get_quote(token->value, a);
	if (split_token(token, where) == -1)
		return (-1);
	if ((type == HEREDOC || type == APPEND)
		&& double_types(where, token) == -1)
		return (-1);
	tmp = new_token(NULL, type, NULL);
	if (!tmp)
		return (-1);
	if (!where && !assess_types_token(&token, tmp))
		return (0);
	shuffle(tmp, token);
	return (0);
}
