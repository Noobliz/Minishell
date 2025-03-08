/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_shuffle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:28:42 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/08 14:42:45 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libparsing.h"
//shuffling a token next to where
void	shuffle(t_token *token, t_token *where)
{	
	if (!where)
		return ;
	token->previous->next = token->next;
	if (token->next)
		token->next->previous = token->previous;
	token->next = where->next;
	token->previous = where;
	if (where->next)
		where->next->previous = token;
	where->next = token;
}
//decides how to shuffle (couldnt remember if we had other concerns that werent PIPE, but if we do let me know ill add that in)
void	cmd_shuffle(t_token *head)
{
	t_token	*token;
	t_token	*last_cmd;

	last_cmd = head;
	if (!head)
		return ;
	head = head->next;
	while (head)
	{
		token = head->next;
		if (head->type == CMD && head->previous->type != CMD)
		{
			shuffle(head, last_cmd);
			last_cmd = head;
		}
		if (head->type == PIPE)
			last_cmd = NULL;
		head = token;
	}
}
