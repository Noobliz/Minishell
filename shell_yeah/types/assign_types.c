/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_types.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:28:42 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 16:00:18 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static void	clean_up(t_token **token)
{
	t_token	*tmp;

	if ((*token)->next)
		tmp = (*token)->next;
	else
		tmp = (*token)->previous;
	delete_token(*token);
	*token = tmp;
}

//first we separate redirs and pipes from the rest of a string
//(if its in a bigger string)
// --careful, if split the special tokens have a NULL value
static int	assign_types_splits(t_token *token)
{
	int	type;

	while (token)
	{
		if (token->type != IGNORE && token->value && !token->value[0])
			clean_up(&token);
		if (token->type != IGNORE && token->value)
		{
			type = get_type(token->value, "<|>");
			if (type != -1 && ((type <= REDIR_OUT && token->value[1])
					|| (type >= APPEND && token->value[2])))
			{
				if (split_type(token, type) == -1)
					return (-1);
				if (token->previous)
					token = token->previous;
			}
		}
		token = token->next;
	}
	return (0);
}

//splits then assigns already cut redirs and pipes
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
		if (token->next && token->next->type != IGNORE
			&& !is_redir(token->next->type, 1) && is_redir(token->type, 0))
			token->next->type = DIR;
		token = token->next;
	}
	return (0);
}
