/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:46:14 by naorakot          #+#    #+#             */
/*   Updated: 2025/02/25 16:14:15 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int	echoing(char *str)
{
	int	i = 0;
	char	*echo = "echo";

	while (echo[i])
		i++;
	i++;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (1);
}

char	**cccv(char **tab, char *str, int add)
{
	char **new_tab;
	int	len;
	int	i;
	len = 0;
	while (tab[len])
		len++;
	new_tab = malloc(len + 1 + add);
	if (!new_tab)
		return (NULL);
	i = 0;
	while (tab[i] && i < len + add)
	{
		new_tab[i] = tab[i];
		i++;
	}
	new_tab[len + add] = NULL;
	return (new_tab);
}

int	find(char *str, char *strstr)
{
	int	i = 0;
	while (strstr[i] && str[i] == strstr[i])
		i++;
	if (strstr[i])
		return (0);
	return (1);
}

int main(int argc, char **argv, char **env)
{
	char *command = readline("ask here : ");
	//char **new_env = cccv(env, NULL, 0);
	while (command && !find(command, "over"))
	{
		/*if (find(&command[i], "export"))
			if (add_to_env(&command[i], new_env) == -1)
				break ;*/
		if (find(command, "echo"))
			if (echoing(command) == -1)
				break ;
		/*if (find(&command[i], "unset"))
			if (sub_from_env(&command[i])== -1)
				break ;*/
		free(command);
		command = readline("ask here : ");
	}
	if (command)
		free(command);
	//free_env(new_env);
}
