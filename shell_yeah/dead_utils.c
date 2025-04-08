/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:17:57 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/03 12:17:58 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbig.h"

// prints tokens
void	print_tokens(t_token *tokens)
{
	printf("\n");
	while (tokens)
	{
		printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

// prints cmds
void	print_cmds(t_cmd *cmd)
{
	int	i;

	while (cmd)
	{
		print("\n");
		printf("cmd :: %s\noptions :: \n", cmd->cmd);
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			printf("%s, ", cmd->argv[i]);
			i++;
		}
		printf("\ninfile : %d, outfile %d\n", cmd->infile, cmd->outfile);
		if (cmd->built_in != -1)
			printf("is builtin\n");
		else
			printf("is not builtin\n");
		cmd = cmd->next;
	}
}

//resets all tokens to CMD type to avoid later confusion;
void	all_cmd_type(t_token **tokens)
{
	t_token	*tmp;
	t_token	*token;

	token = *tokens;
	if (!token)
		return ;
	while (token->previous)
		token = token->previous;
	tmp = token;
	while (tmp)
	{
		tmp = token->next;
		if (token->type != IGNORE && token->value && !token->value[0])
			delete_token(token);
		else if (token->type != IGNORE)
		{
			token->type = CMD;
			*tokens = token;
		}
		if (tmp)
			token = tmp;
	}
	while ((*tokens)->previous)
		*tokens = (*tokens)->previous;
}
