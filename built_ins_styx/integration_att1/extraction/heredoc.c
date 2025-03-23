/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:02:06 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 16:02:22 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

char	*handle_var_str(char *line, t_env *env)
{
	int	i;
	int	j;
	char	*var = NULL;

	i = 0;
	while (line && line[i] && !(line[i] == '$' && (is_alphanum(line[i + 1]) || line[i + 1] == '{')))
		i++;
	if (!line || !line[i])
		return (line);
	j = i + 1;
	j++;
	while (is_alphanum(line[j]))
		j++;
	if (line[i + 1] == '{' && line[j] != '}')
		j = i + 1;
	else if (line[i + 1] == '{')
		j++;
	if (j != i + 1)
	{
		if (line[i + 1] == '{')
			i++;
		var = get_env(&line[i + 1], env);
		if (line[i] == '{')
			i--;
	}
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
//trying out heredoc, pipe ver.
//tried the other version, but couldn't rewind so couldnt read what had been written
//!! will have to add the env so i can take it with me up to here -- added
int	get_heredoc(char *value, t_env *env)
{
	char	*line;
	char	*var;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	line = readline("> ");
	while (!isis(line, value))
	{
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
		line = readline("> ");
	}
	free(line);
	close(pipefd[1]);
	return (pipefd[0]);
}
