/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:23:37 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 15:23:48 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//freeing the linked list of commands
//from head to the end;
void	free_cmds(t_cmd *head)
{
	if (head == NULL)
		return ;
	free_cmds(head->next);
	if (head->argv)
		free_tab(head->argv);
	if (head->cmd)
		free(head->cmd);
	if (head->infile > 0)
		close(head->infile);
	if (head->outfile > 0)
		close(head->outfile);
	free(head);
}

//freeing the linked list of commands
//from head to the beginning;
static void	free_cmds_bw(t_cmd *head)
{
	if (head == NULL)
		return ;
	free_cmds_bw(head->previous);
	if (head->argv)
		free_tab(head->argv);
	if (head->cmd)
		free(head->cmd);
	if (head->infile > 0)
		close(head->infile);
	if (head->outfile > 0)
		close(head->outfile);
	free(head);
}

//new free_cmds from whichever link within the list
void	free_cmds_new(t_cmd *prev, t_cmd *next)
{
	if (!prev && !next)
		return ;
	free_cmds(next);
	free_cmds_bw(prev);
}

//creates a new command link
//all variables set to their untouched versions;
t_cmd	*new_cmd(t_cmd *prev)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->infile = -2;
	cmd->outfile = -2;
	cmd->built_in = -1;
	cmd->next = NULL;
	cmd->previous = prev;
	return (cmd);
}

//freeing a char **, classic
void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
