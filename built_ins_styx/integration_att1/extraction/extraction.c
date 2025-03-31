/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:02:44 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/27 14:45:12 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//alright, so plan is we separate this one into two functions :: one checks files(first, and if issue return immediately)
//one checks commands (second), and so same;

static int	checking_files(t_token *token, t_cmd *cmd, t_data *data)
{
	int check;

	while (token)
	{
		if (token->next && token->next->type == DIR)
			token = token->next;
		if (token->type == DIR)
		{
			if (g_err_code != 130)
				check = get_file(token->previous, cmd, data->env, data);
			else
				return (-2);
			if (check == -1 || check == -2)
				return (check);
		}
		token = token->next;
		if (!token || token->type == PIPE)
			break ;
	}
	return (0);
}

static int	checking_cmd(t_token *token, t_cmd *cmd, char *path)
{
	int	check;

	while (token)
	{
		if (token && token->type == CMD)
		{
			check = get_command(token, cmd, path);
			if (check == -1 || check == -2)
				return (check);
			while (token && token->type == CMD)
				token = token->next;
		}
		if (!token || token->type == PIPE)
			break ;
	}
	return (0);
}

//extracts all the information from token
//up to the next PIPE or until the end;
//-1 malloc error, -2 for "please ignore this one"
static int	assign_cmds(t_token *token, t_cmd *cmd, char *path, t_data *data)
{
	int	check;

	check = checking_files(token, cmd, data);
	if (check < 0)
		return (check);
	check = checking_cmd(token, cmd, path);
	if (check < 0)
		return (check);
	return (0);
}

//extracts all the info from the tokens
//checks the validity of commands and in/outfiles
//creates a cmd link -- then moves to end or next PIPE
int	extraction(t_token *token, t_cmd **prev, char *path, t_data *data)
{
	t_cmd	*cmds;
	int		check;

	if (!token)
		return (0);
	if (token->type == PIPE)
		token = token->next;
	cmds = new_cmd(*prev);
	if (!cmds)
		return (-1);
	if (!*prev)
		*prev = cmds;
	else
		(*prev)->next = cmds;
	check = assign_cmds(token, cmds, path, data);
	if (check == -1)
		return (-1);
	if (check == -2)
		ignore_cmd(cmds);
	while (token && token->type != PIPE)
		token = token->next;
	return (extraction(token, &cmds, path, data));
}
