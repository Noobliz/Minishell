/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:28:59 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/15 09:29:31 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"
#include <sys/stat.h>
#include <sys/types.h>

int	is_directory(char *dir_name, t_cmd *cmd)
{
	struct stat	statbuf;

	if (dir_name[0] != '.' && dir_name[0] != '/')
		return (0);
	if (access(dir_name, F_OK) == -1)
		return (0);
	if (lstat(dir_name, &statbuf) == -1)
	{
		perror("lstat");
		return (-1);
	}
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		print_bash_err(dir_name, "is a directory");
		cmd->built_in = -3;
		return (-2);
	}
	return (0);
}

static int	new_outfile(char *file, int type)
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
static int	close_previous_fds(int type, t_cmd *cmd, t_token *token)
{
	int	fd;

	if (type == REDIR_OUT || type == APPEND)
	{
		if (cmd->outfile > 0)
			close(cmd->outfile);
		return (0);
	}
	if (type == REDIR_IN && cmd->infile != -2)
	{
		fd = open(token->value, O_RDONLY);
		if (fd == -1)
		{
			print_bash_err(token->value, strerror(errno));
			return (-2);
		}
		close(fd);
	}
	return (0);
}

//this is the function for testing out any file
//on error, prints the error and returns -2
int	get_file(t_token *token, t_cmd *cmd)
{
	if (close_previous_fds(token->type, cmd, token->next) == -2)
		return (-2);
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
