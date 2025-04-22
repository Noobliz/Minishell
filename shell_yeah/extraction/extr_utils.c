/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extr_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:49:02 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/25 15:18:16 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//distributes extraction :: heredoc then everything else
int	extraction(t_token *token, t_data *data)
{
	int	check;

	check = extraction_heredoc(token, &(data->cmds), data);
	if (check == -2)
	{
		if (data->cmds)
			free_cmds_new(data->cmds, data->cmds->next);
		data->cmds = NULL;
		return (0);
	}
	if (check == -1)
		return (-1);
	heredoc_check(token, data->cmds);
	check = extraction_pt2(token, data->cmds, data);
	return (check);
}

int	print_bash_err(char *filename, char *err_msg)
{
	print_err("Shell_no! : ");
	print_err(filename);
	print_err(" : ");
	print_err(err_msg);
	print_err("\n");
	return (-2);
}

//checks if the command is a built_in function
//if it is, sets the cmd->built_in to the ref num;
int	is_built_in(char *function, t_cmd *cmd)
{
	if (isis(function, "echo"))
		cmd->built_in = 0;
	else if (isis(function, "cd"))
		cmd->built_in = 6;
	else if (isis(function, "pwd"))
		cmd->built_in = 1;
	else if (isis(function, "export"))
		cmd->built_in = 2;
	else if (isis(function, "unset"))
		cmd->built_in = 3;
	else if (isis(function, "env"))
		cmd->built_in = 4;
	else if (isis(function, "exit"))
		cmd->built_in = 5;
	else
		return (0);
	return (1);
}

//sets argv to NULL so we ignore this command later
//built-in -2 for not found, -3 for dir
void	ignore_cmd(t_cmd *cmd)
{
	if (cmd->argv)
		free_tab(cmd->argv);
	cmd->argv = NULL;
	if (cmd->cmd)
		free(cmd->cmd);
	cmd->cmd = NULL;
	if (cmd->built_in != -3)
		cmd->built_in = -2;
}

//modded len_str for the join_path
int	str_len_path(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != ':')
		i++;
	return (i);
}
