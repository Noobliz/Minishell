/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stick_and_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:18:14 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 20:18:27 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//joins two tokens into one
static int	add_up(t_token *beg, t_token *end)
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
		tmp->type = APPEND;
	if ((sign == 2 || sign == 3)
		&& token->next && token->next->type == IGNORE)
		token->type = HEREDOC;
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
static int	preface_quotes(t_token *token)
{
	t_token	*tmp;

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

//fix_quotes is our new trim_split
//adjusts the quotes if supposed to be part of a bigger token;
int	fix_quotes(t_token *token)
{
	if (preface_quotes(token) == -1)
		return (-1);
	while (token)
	{
		if (token->type == IGNORE && token->next)
		{
			if (token->next->type == IGNORE || token->next->type == APPEND)
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
