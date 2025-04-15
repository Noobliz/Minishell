/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:02:44 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/03 12:47:06 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	checking_files(t_token *token, t_cmd *cmd)
{
	int	check;

	while (token)
	{
		if (token->type == DIR)
		{
			if (g_err_code == 130)
				return (-2);
			check = get_file(token->previous, cmd);
			if (check == -1 || check == -2)
				return (check);
		}
		if (!token || token->type == PIPE)
			break ;
		token = token->next;
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
			if (is_directory(token->value, cmd))
				return (-2);
			check = get_command(token, cmd, path);
			if (check == -1 || check == -2)
				return (check);
			while (token && token->type == CMD)
				token = token->next;
		}
		if (!token || token->type == PIPE)
			break ;
		token = token->next;
	}
	return (0);
}

//extracts all the information from token
//up to the next PIPE or until the end;
//-1 malloc error, -2 for "please ignore this one"
static int	assign_cmds(t_token *token, t_cmd *cmd, t_env *env)
{
	int		check;
	char	*path;

	path = get_env("PATH", env);
	if (!path)
		path = get_env("1PWD", env);
	check = checking_files(token, cmd);
	if (check < 0)
		return (check);
	check = checking_cmd(token, cmd, path);
	if (check < 0)
		return (check);
	if (cmd->cmd && cmd->built_in == -1 && get_env("PATH", env)
		&& isis(cmd->cmd, cmd->argv[0])
		&& cmd->cmd[0] != '/' && cmd->cmd[0] != '.')
	{
		print_bash_err(cmd->cmd, "command not found");
		return (-2);
	}
	return (0);
}

//extracts all the info from the tokens, bar HEREDOC
//checks the validity of commands and in/outfiles
int	extraction_pt2(t_token *token, t_cmd *cmds, t_data *data)
{
	int		check;

	if (!token || !cmds)
		return (0);
	if (token->type == PIPE)
		token = token->next;
	check = assign_cmds(token, cmds, data->env);
	if (check == -1)
		return (-1);
	if (check == -2)
		ignore_cmd(cmds);
	while (token && token->type != PIPE)
		token = token->next;
	check = extraction_pt2(token, cmds->next, data);
	return (check);
}

//all heredocs first
//creates a cmd link -- then moves to end or next PIPE
int	extraction_heredoc(t_token *token, t_cmd **prev, t_data *data)
{
	t_cmd	*cmds;

	if (!token)
		return (0);
	if (token->type == PIPE)
		token = token->next;
	cmds = new_cmd(*prev);
	if (!cmds || (*prev && (*prev)->infile == -1))
		return (-1);
	if (!*prev)
		*prev = cmds;
	else
		(*prev)->next = cmds;
	while (token && token->type != PIPE)
	{
		if (token->type == DIR && token->previous->type == HEREDOC)
		{
			cmds->infile = get_heredoc(cmds->infile,
					token->value, data->env, data);
			if (g_err_code == 130)
				return (-2);
		}
		token = token->next;
	}
	return (extraction_heredoc(token, &cmds, data));
}
