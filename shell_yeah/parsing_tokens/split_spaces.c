/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:18:14 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 20:18:27 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	get_sign(t_token *token, t_token *tmp, int sign)
{
	if (!tmp)
	{
		if (token->value[0] != ' ')
			sign = 1;
		if (last_char(token->value) != ' ')
			sign = sign + 2;
		return (sign);
	}
	if ((sign == 1 || sign == 3)
		&& tmp->previous && tmp->previous->type == IGNORE)
		tmp->sign = 1;
	if ((sign == 2 || sign == 3)
		&& token->next && token->next->type == IGNORE)
		token->sign = token->sign + 2;
	return (0);
}

//splits all inner spaces of a token
//assigns HEREDOC if a non IGNORE needs to be stuck to an IGNORE
static int	split_inner_spaces(t_token *token)
{
	int		sign;
	int		space;
	t_token	*tmp;
	char	*var;

	tmp = token;
	sign = get_sign(token, NULL, 0);
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
	sign = get_sign(token, tmp, sign);
	return (sign);
}

//first we trim and split the non quotes tokens
//turning them into HEREDOC should they be added with an IGNORE token;
static int	preface_quotes(t_token **tokens)
{
	t_token	*tmp;
	t_token *token;

	token = *tokens;
	tmp = token;
	while (tmp)
	{
		tmp = token->next;
		if (token->type != IGNORE)
		{
			if (split_inner_spaces(token) == -1)
				return (-1);
		}
		if (tmp)
			token = tmp;
	}
	while (token->previous)
		token = token->previous;
	*tokens = token;
	return (0);
}

//fix_quotes is our new trim_split
//adjusts the quotes if supposed to be part of a bigger token;
int	split_and_sign(t_token **tokens)
{
	t_token	*tmp;
	t_token	*token;

	if (preface_quotes(tokens) == -1)
		return (-1);
	token = *tokens;
	tmp = token;
	while (tmp)
	{
		tmp = token->next;
		if (token->type == IGNORE && token->next
			&& token->next->type == IGNORE)
			token->sign = 2;
		if (token->type != IGNORE && token->value && !token->value[0])
			delete_token(token);
		else
			*tokens = token;
		token = tmp;
	}
	while ((*tokens)->previous)
		*tokens = (*tokens)->previous;
	return (0);
}
