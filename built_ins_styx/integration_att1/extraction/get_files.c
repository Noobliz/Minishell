/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:28:59 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 15:29:16 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//this one just creates a new outfile
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
	if (type == REDIR_IN || type == HEREDOC)
	{
		if (cmd->infile > 0)
			close(cmd->infile);
	}
	else
	{
		if (cmd->outfile > 0)
			close(cmd->outfile);
	}
}

//this is the function for testing out any file
//based on the REDIR before it in the tokens;
//on error, prints the error and returns -2
//(to signal to ignore that function);
int	get_file(t_token *token, t_cmd *cmd, t_env *env)
{
	close_previous_fds(token->type, cmd);
	if (token->type == HEREDOC)
		cmd->infile = get_heredoc(token->next->value, env);
	if (token->type == REDIR_IN)
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
		print_bash_err(token->next->value, strerror(errno));
		return (-2);
	}
	return (0);
}
