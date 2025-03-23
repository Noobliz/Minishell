/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:53:32 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 11:53:34 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

void	free_tokens(t_token *token)
{
	if (!token)
		return ;
	free_tokens(token->next);
	if (token->value)
		free(token->value);
	free(token);
}

//in case of empty quotes :: "" or ''
int	empty_quote(t_token *token)
{
	char	*str;

	str = copy(&token->value[1]);
	if (!str)
		return (-1);
	free(token->value);
	token->value = copy("");
	if (!token->value)
	{
		free(str);
		return (-1);
	}
	token->next = new_token(str, CMD, token);
	if (!token->next)
	{
		free(str);
		return (-1);
	}
	return (0);
}

//same as your create_tokens, except no strdup and i've added the previous token
t_token	*new_token(char *value, t_type type, t_token *prev)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->next = NULL;
	new->previous = prev;
	return (new);
}

//takes the value and finds the index of the quote, or -1 if no quote
int	get_quote(char *token, char quote)
{
	int	i;

	i = 0;
	while (token[i] && token[i] != quote)
		i++;
	if (token[i])
		return (i);
	return (-1);
}
