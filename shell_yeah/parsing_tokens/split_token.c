/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:58:05 by naorakot          #+#    #+#             */
/*   Updated: 2025/04/08 15:53:44 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static char	**mallocking(char *str, int quote)
{
	char	**res;
	int		i;

	res = malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	res[0] = malloc(quote + 1);
	if (!res[0])
	{
		free(res);
		return (NULL);
	}
	i = 0;
	while (str[i + 1 + quote])
		i++;
	res[1] = malloc(i + 1);
	if (!res[1])
	{
		free(res[0]);
		free(res);
		return (NULL);
	}
	res[2] = NULL;
	return (res);
}

//splits once at quote, excludes str[quote]
char	**split_once(char *str, int quote)
{
	char	**res;
	int		i;

	res = mallocking(str, quote);
	if (!res)
		return (NULL);
	i = 0;
	while (i < quote)
	{
		res[0][i] = str[i];
		i++;
	}
	res[0][i] = '\0';
	i = 0;
	while (str[i + 1 + quote])
	{
		res[1][i] = str[i + 1 + quote];
		i++;
	}
	res[1][i] = '\0';
	return (res);
}

//in case of quote at the beginning of token->value
//moves right
static int	null_quote(t_token *token)
{
	char	*str;

	str = copy(&token->value[1]);
	if (!str)
		return (-1);
	free(token->value);
	token->value = str;
	return (0);
}

//splits the token at the quote index into
//two separate tokens (quote excluded)
int	split_token(t_token *token, int quote)
{
	char	**splat;
	t_token	*item;

	if (quote == 0)
		return (null_quote(token));
	item = NULL;
	splat = split_once(token->value, quote);
	if (!splat)
		return (-1);
	item = new_token(splat[1], CMD, token);
	if (!item)
		return (-1);
	free(token->value);
	token->value = splat[0];
	free(splat);
	if (token->sign == 2 || token->sign == 3)
		item->sign = 2;
	if (token->sign == 2 || token->sign == 3)
		token->sign = token->sign - 2;
	item->next = token->next;
	if (token->next)
		token->next->previous = item;
	token->next = item;
	return (0);
}
