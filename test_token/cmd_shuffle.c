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
//shuffling a token next to where, fully protected so it can be used in other contexts
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
//splits the token and adds the new type_token either in between or before it
int	split_type(t_token *token, int type)
{
	int	where;
	char	a;
	t_token	*tmp;

	if (type == PIPE)
		a = '|';
	if (type == REDIR_IN)
		a = '<';
	if (type == REDIR_OUT)
		a = '>';
	where = get_quote(token->value, a);
	if (split_token(token, where) == -1)
		return (-1);
	tmp = new_token(NULL, type, NULL);
	if (!tmp)
		return (-1);
	if (!where)
	{
		if (!token->previous)
		{
			token->previous = tmp;
			tmp->next = token;
			return (0);
		}
		else
			token = token->previous;
	}
	shuffle(tmp, token);
	return (0);
}
//gets the type of character found in the string given
int	get_char(char *str, char *types)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == types[0])
			return (REDIR_IN);
		if (str[i] == types[1])
			return (PIPE);
		if (str[i] == types[3])
			return (REDIR_OUT);
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
//this one is supposed to assign types (and separate them from the rest of a string if its in it)
// --careful, i made it so the special tokens have a NULL value (didnt see the point when the type says it all), so that needs to be taken into account when freeing the whole list
// --also keep in mind you might need to adjust the list's head after this function given i can add tokens before the head given to me (i can fix that by taking a pointer of pointer if that's not comfortable for you)
int	assign_types(t_token *token)
{
	int	type;
	t_token	*tmp;

	while (token)
	{
		if (token->type != IGNORE)
		{
			type = get_char(token->value, "<|>");
			if (type != -1)
			{
				if (token->value[1] == '\0')
					break ;
				if (split_type(token, type) == -1)
					return (-1);
			}
		}
		token = token->next;
	}
	while (token->previous)
		token = token->previous;
	while (token)
	{
		if (token->type != IGNORE && get_char(token->value, "<|>"))
			token->type = get_char(token->value, "<|>");
		if (token->next && token->type == REDIR_OUT
			&& token->next->type == REDIR_OUT)
		{
			token->type = APPEND;
			del_token(token->next);
		}
		if (token->next && token->type == REDIR_IN
			&& token->next->type == REDIR_IN)
		{
			token->type = HEREDOC;
			del_token(token->next);
		}
		token = token->next;
	}
	return (0);
}
//edit :: i forgot to take out the spaces in the non-ignore tokens :: split it then go, before doing the del_spaces bit
