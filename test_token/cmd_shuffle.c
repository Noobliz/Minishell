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
//decides how to shuffle (couldnt remember if we had other concerns that werent PIPE, but if we do let me know and ill add that in)
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
//for HEREDOC and APPEND, gets rid of the second < or >
int	double_types(int where, t_token *token)
{
	char	*value;

	if (where)
		token = token->next;
	value = copy(&token->value[1]);
	if (!value)
		return (-1);
	free(token->value);
	token->value = value;
	return (0);
}
//splits the token and adds the new type_token either in between or before it
int	split_type(t_token *token, int type)
{
	int	where;
	char	a;
	t_token	*tmp;

	if (type == PIPE)
		a = '|';
	if (type == REDIR_IN || type == HEREDOC)
		a = '<';
	if (type == REDIR_OUT || type == APPEND)
		a = '>';
	where = get_quote(token->value, a);
	if ((type == HEREDOC || type == APPEND) && !token->value[where + 1])
		return (0);
	if (split_token(token, where) == -1)
		return (-1);
	if ((type == HEREDOC || type == APPEND) && double_types(where, token) == -1)
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
int	assign_types(t_token **head)
{
	int	type;
	t_token	*token;

	token = *head;
	while (token)
	{
		if (token->type != IGNORE && token->value)
		{
			type = get_char(token->value, "<|>");
			if (type != -1 && token->value[1])
			{
				if (split_type(token, type) == -1)
					return (-1);
			}
		}
		token = token->next;
	}
	token = *head;
	while (token && token->previous)
		token = token->previous;
	*head = token;
	while (token)
	{
		if (token->type != IGNORE && get_char(token->value, "<|>") != -1)
			token->type = get_char(token->value, "<|>");
		if (token->type == IGNORE)
			token->type = CMD;
		if (token->next && token->type != DIR && token->type != CMD && token->type != PIPE)
			token->next->type = DIR;
		token = token->next;
	}
	return (0);
}
/*
"ls" "-l"
s_all {
	char	**cmds [0] = "/bin/ls";
	char 	*cmd = "ls";
	int	infile = fd = open("");
	int	outfile = fd;
}

while (head && head->type != PIPE)
	head = head->next;
head = head->next;
if (!head)
	return ;

token :: "<", "infile", "out>>>file" (cmd)
token :: "<" (cmd)
token :: "infile" (cmd)
token :: "out" (cmd)
token :: NULL (REDIR_OUT)
token :: NULL (REDIR_OUT)
token :: "file" (cmd)

token :: "<" (REDIR_IN)
token :: NULL (PIPE)
token :: NULL (APPEND)
token :: NULL (REDIR_OUT)
token :: "file"
//edit :: i forgot to take out the spaces in the non-ignore tokens :: split it then go, before doing the del_spaces bit*/
