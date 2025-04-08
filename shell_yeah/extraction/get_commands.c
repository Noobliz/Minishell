/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:38:37 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/31 13:32:09 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

// modded join for specific path needs
static char	*join_path(char *s, char *s2)
{
	int		len;
	int		i;
	char	*str;

	if (!s && !s2)
		return (NULL);
	str = malloc(str_len_path(s) + len_str(s2) + 2);
	if (!str)
		return (NULL);
	i = 0;
	while (s && s[i] && s[i] != ':')
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '/';
	i++;
	len = 0;
	while (s2 && s2[len])
	{
		str[i + len] = s2[len];
		len++;
	}
	str[i + len] = '\0';
	return (str);
}

// finds the command if it's not a built_in
// through PATH or 1PWD
static int	find_command(char *paths, char **command)
{
	int		i;
	char	*com;

	i = -1;
	while (*(command)[0] && paths && paths[i + 1])
	{
		if (i == -1 || paths[i] == ':')
		{
			com = join_path(&paths[i + 1], *command);
			if (!com)
				return (-1);
			if (access(com, F_OK) == 0)
			{
				free(*command);
				*command = com;
				return (0);
			}
			free(com);
		}
		i++;
	}
	if (access(*command, F_OK) == -1)
		return (-2);
	return (0);
}

// fills the argv with copies of all the command's options
static char	**fill_argv(char *first, t_token *token, char **tab)
{
	int	i;

	tab[0] = first;
	i = 1;
	tab[1] = NULL;
	while (token && token->type == CMD)
	{
		tab[i + 1] = NULL;
		tab[i] = copy(token->value);
		if (!tab[i])
		{
			free_tab(tab);
			return (NULL);
		}
		i++;
		token = token->next;
	}
	return (tab);
}

// creates the argv variable by counting the amount of options
// then fills it with above function;
static char	**get_argv(char *first, t_token *token)
{
	int		ct;
	char	**tab;
	t_token	*tmp;

	ct = 0;
	tmp = token->next;
	while (token && token->type == CMD)
	{
		ct++;
		token = token->next;
	}
	tab = malloc(sizeof(char *) * (ct + 1));
	if (!tab)
		return (NULL);
	tab = fill_argv(first, tmp, tab);
	return (tab);
}

// sets the variables cmd->cmd, cmd->argv
//(cmd->built_in if built_in)
// returns -1 on malloc issues, -2 if the cmd should be ignored
int	get_command(t_token *token, t_cmd *cmd, char *path)
{
	char	*str;
	int		check;

	check = 0;
	str = copy(token->value);
	if (!str)
		return (-1);
	if (!is_built_in(token->value, cmd))
		check = find_command(path, &str);
	if (cmd->built_in == -1 && access(str, X_OK) == -1)
		check = print_bash_err(token->value, "permission denied");
	else if (check == -2)
		print_bash_err(token->value, "command not found");
	if (check == -1 || check == -2)
	{
		free(str);
		return (check);
	}
	cmd->cmd = copy(token->value);
	if (!cmd->cmd)
		return (-1);
	cmd->argv = get_argv(str, token);
	if (!cmd->argv)
		return (-1);
	return (0);
}
