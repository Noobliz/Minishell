/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:23:10 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/23 13:23:13 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libbig.h"

static int	check_forbidden(t_token *token)
{
	int	i;

	while (token)
	{
		if (token->type != IGNORE && token->value)
		{
			i = 0;
			while (token->value[i])
			{
				if (forbidden(token->value[i]))
					return (-2);
				i++;
			}
		}
		token = token->next;
	}
	return (0);
}

int	spec_check(t_token *token)
{
	if (check_forbidden(token) == -2)
		return (-2);
	if (token && token->type == PIPE)
		return (check_src(PIPE, CMD));
	while (token)
	{
		if (token->type == PIPE
			&& token->next && token->next->type == PIPE)
			return (check_src(token->type, token->next->type));
		if (is_redir(token->type, 1))
		{
			if (!token->next)
			{
				print_syntax_err("newline");
				return (-2);
			}
			if (token->type != PIPE && is_redir(token->next->type, 1))
				return (check_src(token->type, token->next->type));
		}
		token = token->next;
	}
	return (0);
}
