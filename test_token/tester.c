/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:57:10 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/04 19:00:05 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libparsing.h"
#include <stdio.h>

void  free_tokens(t_token *token)
{
  if (!token)
    return ;
  free_tokens(token->next);
  free(token->value);
  free(token);
}

void	disp_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->var);
		env = env->next;
	}
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
        // if(tokens && tokens->previous)
        //     printf("previous token value = %s previous token type = %d\n", tokens->previous->value, tokens->previous->type);
        tokens = tokens->next;
    }
}

int main(void)
{
	t_env	*n_env = env_item("PATH=here", 0);
	n_env->next = env_item("NUM=herealso", 0);
	int i = 0;
	t_token *token = NULL;
	char  *str = copy("$PATH value \'$PATH and  $NUM\' more \"   $PATH of $NUM    \" over there $NUM");

	//if (argc > 1 || argv[0][0] == '\0')
		//return (0);
	if (!str)
	  return (0);
	token = new_token(str, CMD, NULL);
	if (!token)
	  return (0);
        i = parsing_pt_2(token, n_env);
	if (i == -1)
	  printf("malloc error\n");
	if (i == -2)
	  printf("only one quote\n");
	i = trim_tokens(token);
	if (i == -1)
	  printf("malloc error\n");
	token->next->type = REDIR_OUT;
	token->next->next->next->type = PIPE;
	cmd_shuffle(token);
        print_tokens(token);
        free_tokens(token);
        free(n_env->next);
        free(n_env);
	return (0);
}
