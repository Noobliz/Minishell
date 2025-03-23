/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extr_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:49:02 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 15:49:14 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//prints out the error message like so :: "bash : filename : error_msg\n"
void	print_bash_err(char *filename, char *err_msg)
{
	print("bash : ");
	print(filename);
	print(" : ");
	print(err_msg);
	print("\n");
}

//checks if the command is a built_in function
//if it is, sets the cmd->built_in to the ref num;
int	is_built_in(char *function, t_cmd *cmd)
{
	if (isis(function, "echo"))
		cmd->built_in = 0;
	else if (isis(function, "cd"))
		cmd->built_in = 5;
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
void	ignore_cmd(t_cmd *cmd)
{
	if (cmd->argv)
		free_tab(cmd->argv);
	cmd->argv = NULL;
}

//adding the total_count --unused rn
void	add_count_cmds(t_cmd *cmd)
{
	int		ct;
	t_cmd	*tmp;

	tmp = cmd;
	ct = 0;
	while (cmd)
	{
		ct++;
		cmd = cmd->next;
	}
	while (tmp)
	{
		tmp->total = ct;
		tmp = tmp->next;
	}
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
