/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:02:06 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/26 17:40:20 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//decides where we cut off
static int	set_j(char *line, int i)
{
	int	j;

	j = i + 1;
	j++;
	while (is_alphanum(line[j]))
		j++;
	if (line[i + 1] == '{' && line[j] != '}')
		j = i + 1;
	else if (line[i + 1] == '{')
		j++;
	return (j);
}

//finds var if var there is
static char	*get_var(char *line, t_env *env, int i, int j)
{
	char	*var;

	var = NULL;
	if (j != i + 1)
	{
		if (line[i + 1] == '{')
			i++;
		var = get_env(&line[i + 1], env);
	}
	return (var);
}

//takes a string, returns it with the appropriate vars updated
char	*handle_var_str(char *line, t_env *env)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	while (line && line[i]
		&& !(line[i] == '$'
			&& (is_alphanum(line[i + 1]) || line[i + 1] == '{')))
		i++;
	if (!line || !line[i])
		return (line);
	j = set_j(line, i);
	var = get_var(line, env, i, j);
	if (var)
		var = replace(line, var, i, j);
	else
		var = replace(line, "", i, j);
	if (!var)
	{
		free(line);
		return (NULL);
	}
	free(line);
	return (handle_var_str(var, env));
}

//heredoc per line
static int	get_heredoc_inloop(char *line, t_env *env, int pipefd[2])
{
	char	*var;

	if (get_quote(line, '$') != -1)
	{
		var = handle_var_str(line, env);
		if (!var)
		{
			perror("malloc err");
			return (-1);
		}
		line = var;
	}
	write(pipefd[1], line, len_str(line));
	write(pipefd[1], "\n", 1);
	free(line);
	return (0);
}

//trying out heredoc, pipe ver.
//tried the other version, but couldn't rewind
//!! will have to add the env so i can take it with me up to here -- added
int	get_heredoc(char *value, t_env *env)
{
	char	*line;
	int		pipefd[2];
	int		pid;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, &sig_handler_heredoc);
		line = readline("> ");
		signal(SIGQUIT, &sig_do_nothing);
		signal(SIGINT, &sig_do_nothing);
		if (g_err_code == 130)
		{
			// remarques-tu la petite ligne vide qui s'ajoute quand tu ctrl C dans un heredoc?
			free(line);
			close(pipefd[1]);
			exit(21);
			//return (pipefd[0]);
		}
		while (line && !isis(line, value))
		{
			if (get_heredoc_inloop(line, env, pipefd) == -1)
				return (-1);
			signal(SIGQUIT, SIG_IGN);
			signal(SIGINT, &sig_handler_heredoc);
			line = readline("> ");
			signal(SIGQUIT, &sig_do_nothing);
			signal(SIGINT, &sig_do_nothing);
			if (g_err_code == 130)
			{
				free(line);
				close(pipefd[1]);
				//printf("%d\n", g_err_code);
				exit(21);
				//return (pipefd[0]);
			}
		}
		free(line);
		exit (0);
	}
	int status = 0;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 21)
		{
			g_err_code = 130;
			//sert a mettre a jour le g err code dans le parent
			//ne pas oublier de mettre a jour last exit code
			// attention, ne pas reinitialiser g err code ici, pcq ca empeche l exec apres
		}
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
