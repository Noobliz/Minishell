/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_types.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:28:42 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 14:09:11 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//first we separate redirs and pipes from the rest of a string
//(if its in a string)
// --careful, if split the special tokens have a NULL value
static int	assign_types_splits(t_token *token)
{
	int	type;

	while (token)
	{
		if (token->type != IGNORE && token->value && !token->value[0])
		{
			token = token->next;
			delete_token(token->previous);
		}
		if (token->type != IGNORE && token->value)
		{
			type = get_type(token->value, "<|>");
			if (type != -1 && ((type <= REDIR_OUT && token->value[1])
					|| (type >= APPEND && token->value[2])))
			{
				if (split_type(token, type) == -1)
					return (-1);
			}
		}
		token = token->next;
	}
	return (0);
}

//this one is supposed to assign all types once split
int	assign_types(t_token **head)
{
	t_token	*token;

	token = *head;
	if (assign_types_splits(*head) == -1)
		return (-1);
	while (token && token->previous)
		token = token->previous;
	*head = token;
	while (token)
	{
		if (token->type != IGNORE && get_type(token->value, "<|>") != -1)
			token->type = get_type(token->value, "<|>");
		if (token->type == IGNORE)
			token->type = CMD;
		if (token->next && token->type != DIR
			&& token->type != CMD && token->type != PIPE)
			token->next->type = DIR;
		token = token->next;
	}
	return (0);
}
