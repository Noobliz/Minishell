/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:02:44 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/26 17:11:48 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//extracts all the information from token
//up to the next PIPE or until the end;
//-1 malloc error, -2 for "please ignore this one"
int	assign_cmds(t_token *token, t_cmd *cmd, char *path, t_env *env)
{
	int	check;

	while (token)
	{
		if (token->next && token->next->type == DIR)
			token = token->next;
		if (token->type == DIR)
		{
			if (g_err_code != 130)
				check = get_file(token->previous, cmd, env);
			else
				return (-2);
			if (check == -1 || check == -2)
				return (check);
			token = token->next;
		}
		if (token && token->type == CMD)
		{
			check = get_command(token, cmd, path);
			if (check == -1 || check == -2)
				return (check);
			while (token && token->type == CMD)
				token = token->next;
		}
		if (!token || token->type == PIPE)
			return (0);
	}
	return (0);
}

//extracts all the info from the tokens
//checks the validity of commands and in/outfiles
//creates a cmd link -- then moves to end or next PIPE
int	extraction(t_token *token, t_cmd **prev, char *path, t_env *env)
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
	check = assign_cmds(token, cmds, path, env);
	if (check == -1)
		return (-1);
	if (check == -2)
		ignore_cmd(cmds);
	if (!*prev)
		*prev = cmds;
	else
		(*prev)->next = cmds;
	while (token && token->type != PIPE)
		token = token->next;
	return (extraction(token, &cmds, path, env));
}
