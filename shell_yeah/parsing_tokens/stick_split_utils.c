/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stick_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:19:48 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 20:20:03 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

//getting the last character of a string
char	last_char(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return ('\0');
	while (str[i])
		i++;
	return (str[i - 1]);
}

void	delete_token(t_token *token)
{
	if (!token)
		return ;
	if (token->previous)
		token->previous->next = token->next;
	if (token->next)
		token->next->previous = token->previous;
	if (token->value)
		free(token->value);
	free(token);
}

static int	set_j(char *str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	j = i;
	while (str[i] && str[i + 1])
		i++;
	while (i && str[i] == ' ')
	{
		i--;
		j++;
	}
	return (j);
}

//getting rid of spaces in the appropriate tokens here (token->type == CMD)
char	*del_spaces(char *str)
{
	char	*s;
	int		i;
	int		j;
	int		len;

	j = set_j(str);
	len = len_str(str) - j;
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] == ' ')
		i++;
	while (j < len)
	{
		s[j] = str[i + j];
		j++;
	}
	s[j] = '\0';
	return (s);
}
