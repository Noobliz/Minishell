/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stick_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:19:48 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 15:54:20 by lguiet           ###   ########.fr       */
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
	while (str[i] && sp(str[i]))
		i++;
	j = i;
	while (str[i] && str[i + 1])
		i++;
	while (i && sp(str[i]))
	{
		i--;
		j++;
	}
	return (j);
}

//gets rid of spaces in the CMD tokens
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
	while (sp(str[i]))
		i++;
	while (j < len)
	{
		s[j] = str[i + j];
		j++;
	}
	s[j] = '\0';
	return (s);
}

int	sp(int a)
{
	if (a <= 13 && a >= 9)
		return (1);
	if (a == 32)
		return (1);
	return (0);
}
