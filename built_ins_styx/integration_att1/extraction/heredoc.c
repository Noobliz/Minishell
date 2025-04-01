/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:02:06 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/27 15:09:30 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	done_heredoc(char *line, int pipefd[2], t_data *data, int code)
{
	free(line);
	close(pipefd[1]);
	close(pipefd[0]);
	free_all_things(data);
	if (code == -1)
		code = errno;
	return (code);
}

static char	*get_heredoc_line(void)
{
	char	*line;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler_heredoc);
	line = readline("> ");
	signal(SIGQUIT, &sig_do_nothing);
	signal(SIGINT, &sig_do_nothing);
	return (line);
}

static int	heredoc_child(int pipefd[2], t_env *env, t_data *data, char *value)
{
	char	*line;

	line = get_heredoc_line();
	// remarques-tu la petite ligne vide qui s'ajoute quand tu ctrl C dans un heredoc?
	if (g_err_code == 130)
		return(done_heredoc(line, pipefd, data, 21));
	while (line && !isis(line, value))
	{
		if (get_heredoc_inloop(line, env, pipefd, data->last_exit_code) == -1)
			return (done_heredoc(line, pipefd, data, -1));
		line = get_heredoc_line();
		if (g_err_code == 130)
			return(done_heredoc(line, pipefd, data, 21));
	}
	return(done_heredoc(line, pipefd, data, 0));
}

static int	heredoc_setup(int *pid, t_data *data, int pipefd[2])
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		close(pipefd[1]);
		close(pipefd[0]);
		data->last_exit_code = -1;
		return (-1);
	}
	return (0);
}

//trying out heredoc, pipe ver.
//tried the other version, but couldn't rewind
//!! will have to add the env so i can take it with me up to here -- added
int	get_heredoc(char *value, t_env *env, t_data *data)
{
	int		pipefd[2];
	int		pid;
	int		status;

	pid = 0;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		data->last_exit_code = -1;
		return (-1);
	}
	if (heredoc_setup(&pid, data, pipefd) == -1)
		return (-1);
	if (pid == 0)
		exit(heredoc_child(pipefd, env, data, value));
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 21)
		g_err_code = 130;
	//sert a mettre a jour le g err code dans le parent
	//ne pas oublier de mettre a jour last exit code
	// attention, ne pas reinitialiser g err code ici, pcq ca empeche l exec apres
	close(pipefd[1]);
	return (pipefd[0]);
}
