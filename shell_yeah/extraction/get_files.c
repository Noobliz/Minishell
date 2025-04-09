/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:28:59 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/27 14:43:54 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

int	new_outfile(char *file, int type)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY, 00644);
	if (fd == -1)
		return (-1);
	close(fd);
	if (type == REDIR_OUT)
		fd = open(file, O_WRONLY | O_TRUNC);
	else
		fd = open(file, O_WRONLY | O_APPEND);
	return (fd);
}

//closes previously opened infiles and outfiles
void	close_previous_fds(int type, t_cmd *cmd)
{
	if (type == REDIR_OUT || type == APPEND)
	{
		if (cmd->outfile > 0)
			close(cmd->outfile);
	}
}

//this is the function for testing out any file
//on error, prints the error and returns -2
int	get_file(t_token *token, t_cmd *cmd)
{
	close_previous_fds(token->type, cmd);
	if (token->type == REDIR_IN && cmd->infile == -2)
		cmd->infile = open(token->next->value, O_RDONLY);
	if (token->type == REDIR_OUT)
	{
		cmd->outfile = open(token->next->value, O_WRONLY | O_TRUNC);
		if (cmd->outfile == -1
			&& access(token->next->value, F_OK) == -1)
			cmd->outfile = new_outfile(token->next->value, REDIR_OUT);
	}
	if (token->type == APPEND)
	{
		cmd->outfile = open(token->next->value, O_WRONLY | O_APPEND);
		if (cmd->outfile == -1
			&& access(token->next->value, F_OK) == -1)
			cmd->outfile = new_outfile(token->next->value, APPEND);
	}
	if (cmd->infile == -1 || cmd->outfile == -1)
	{
		if (token->type != HEREDOC)
			print_bash_err(token->next->value, strerror(errno));
		return (-2);
	}
	return (0);
}
