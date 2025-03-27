/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:48:06 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 13:48:16 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//shuffling a token next to where
//fully protected so it can be used in other contexts
void	shuffle(t_token *token, t_token *where)
{
	if (!where)
		return ;
	if (token->previous)
		token->previous->next = token->next;
	if (token->next)
		token->next->previous = token->previous;
	token->next = where->next;
	token->previous = where;
	if (where->next)
		where->next->previous = token;
	where->next = token;
}

//decides how to shuffle
void	cmd_shuffle(t_token *head)
{
	t_token	*token;
	t_token	*last_cmd;

	last_cmd = head;
	while (last_cmd && last_cmd->type != CMD)
		last_cmd = last_cmd->next;
	if (!head || !last_cmd)
		return ;
	head = last_cmd->next;
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

//gets the type of character found in the string given
int	get_type_char(char *str, char *types)
{
	int	i;

	i = 0;
	if (str[i] == types[0])
	{
		if (str[i + 1] == types[0])
			return (HEREDOC);
		else
			return (REDIR_IN);
	}
	if (str[i] == types[1])
		return (PIPE);
	if (str[i] == types[2])
	{
		if (str[i + 1] == types[2])
			return (APPEND);
		else
			return (REDIR_OUT);
	}
	return (-1);
}

int	get_type(char *str, char *types)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == types[0] || str[i] == types[1] || str[i] == types[2])
			return (get_type_char(&str[i], types));
		i++;
	}
	return (-1);
}

//deletes a token
void	del_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}
