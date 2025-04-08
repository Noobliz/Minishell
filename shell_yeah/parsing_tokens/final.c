/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:18:14 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 15:44:24 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//joins two tokens into one, keeps relevant type+signs
static int	add_up(t_token **begs, t_token *end)
{
	char	*str;
	t_token	*beg;

	beg = *begs;
	if (is_redir(beg->type, 1) || is_redir(end->type, 1))
	{
		*begs = beg->next;
		return (0);
	}
	str = join(beg->value, end->value);
	if (!str)
		return (-1);
	free(beg->value);
	free(end->value);
	beg->value = str;
	beg->next = end->next;
	beg->sign = end->sign;
	if (end->type != IGNORE)
		beg->type = end->type;
	if (end->next)
		end->next->previous = beg;
	free(end);
	return (0);
}

int	stick_quotes(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		if ((token->sign == 2 || token->sign == 3)
			|| (next && (next->sign == 1 || next->sign == 3)))
		{
			if (add_up(&token, next) == -1)
				return (-1);
		}
		else
			token = token->next;
	}
	return (0);
}

void	final_types(t_token *token)
{
	while (token)
	{
		if (token->type == IGNORE)
		{
			if (token->previous && is_redir(token->previous->type, 0))
				token->type = DIR;
			else
				token->type = CMD;
		}
		token = token->next;
	}
}
