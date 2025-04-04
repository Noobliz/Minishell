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

//for every token, a spec check
static int	spec_check_inloop(t_token *token, int *type, int *type2)
{
	int	i;

	i = 0;
	while (token->value && token->value[i] != '\0')
	{
		if (forbidden(token->value[i]))
			return (-2);
		*type2 = get_type_char(&token->value[i], "<|>");
		if (*type2 != -1)
		{
			if (*type != -1 && *type != PIPE)
				return (check_src(*type2, token->value[i + 1]));
			*type = *type2;
			if (*type2 == APPEND || *type2 == HEREDOC)
				i++;
		}
		else
			*type = -1;
		i++;
	}
	return (0);
}

//this function does every syntax check to do with special characters
//forbidden chars, positions;
//it checks that a REDIR type has a file given,
//and that a PIPE is not at the beginning or end of the line;
int	spec_check(t_token *token)
{
	int	type;
	int	type2;

	type2 = -1;
	type = -1;
	if (token && token->type != IGNORE && token->value[0] == '|')
		return (check_src(PIPE, 'a'));
	while (token)
	{
		if (token->type != IGNORE
			&& spec_check_inloop(token, &type, &type2) == -2)
			return (-2);
		else
		{
			type = -1;
			type2 = -1;
		}
		if (!token->next && (type2 != -1 || is_redir(token->type, 1)))
		{
			print_syntax_err("newline");
			return (-2);
		}
		token = token->next;
	}
	return (0);
}
