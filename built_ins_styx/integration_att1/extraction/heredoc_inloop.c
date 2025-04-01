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

//decides where we cut off
static int	set_j(char *line, int i)
{
	int	j;

	j = i + 1;
	j++;
	//if (str[i + 1] == '{' && str[i + 2] == '?' && str[i + 3] == '}')
		//return (j + 2);
	if (line[i + 1] == '?')
		return (j);
	while (is_alphanum(line[j]))
		j++;
	/*if (line[i + 1] == '{' && line[j] != '}')
		j = i + 1;
	else if (line[i + 1] == '{')
		j++;*/
	return (j);
}

//finds var if var there is
static char	*get_var(char *line, t_env *env, int i, int j)
{
	char	*var;

	var = NULL;
	if (line[i + 1] == '?')
		return (var);
	if (j != i + 1)
	{
		/*if (line[i + 1] == '{')
			i++;*/
		var = get_env(&line[i + 1], env);
	}
	return (var);
}

//takes a string, returns it with the appropriate vars updated
char	*handle_var_str(char *line, t_env *env, char *lec)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	while (line && line[i]
		&& !(line[i] == '$'
			&& (is_alphanum(line[i + 1]) /*|| line[i + 1] == '{' */|| line[i + 1] == '?')))
		i++;
	if (!line || !line[i])
		return (line);
	j = set_j(line, i);
	var = get_var(line, env, i, j);
	if (line[i + 1] == '?')
		var = lec;
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
	return (handle_var_str(var, env, lec));
}

//heredoc per line
int	get_heredoc_inloop(char *line, t_env *env, int pipefd[2], int code)
{
	char	*var;
	char	*lec;

	lec = num_str(code);
	if (!lec)
		return (-1);
	if (get_quote(line, '$') != -1)
	{
		var = handle_var_str(line, env, lec);
		if (!var)
		{
			perror("malloc err");
			free(lec);
			free(line);
			return (-1);
		}
		line = var;
	}
	write(pipefd[1], line, len_str(line));
	write(pipefd[1], "\n", 1);
	free(line);
	free(lec);
	return (0);
}
