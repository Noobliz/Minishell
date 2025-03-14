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

#include "libextract.h"
#include <stdio.h>

void  free_tokens(t_token *token)
{
  if (!token)
    return ;
  free_tokens(token->next);
  if (token->value)
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
    printf("\n");
    while (tokens)
    {
        printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

void print_cmds(t_cmd *cmd)
{
  int i;
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
    if (cmd->built_in)
      printf("is builtin\n");
    else
      printf("is not builtin\n");
    cmd = cmd->next;
  }
}

int main(void)
{
	t_env	*n_env = env_item("PATH=here", 0);
	n_env->next = env_item("NUM=herealso", 0);
	int i = 0;
	t_token *token = NULL;
	char  *str = copy("\"\" output.txt");
	t_cmd *head = NULL;
	char *path = "/home/styx/.local/bin:/home/styx/.local/bin:/home/styx/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin";

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
	i = trim_split_tokens(token);
	if (i == -1)
	  printf("malloc error\n");
	print_tokens(token);
        spec_check(token);
        if (assign_types(&token) == -1 || !token)
          printf("malloc error\n");
        cmd_shuffle(token);
        extraction(token, &head, path);
        print_tokens(token);
        print_cmds(head);
        free_tokens(token);
        free_cmds(head);
        free(n_env->next);
        free(n_env);
	return (0);
}

/*int main (void)
{
  t_cmd *head = NULL;
  t_token *token;
  char *path = "/home/styx/.local/bin:/home/styx/.local/bin:/home/styx/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin";
  
  token = new_token(copy("wc"), CMD, NULL);
  token->next = new_token(copy("-l"), CMD, token);
  token->next->next = new_token(NULL, APPEND, token->next);
  token->next->next->next = new_token(copy("file2"), DIR, token->next->next);
  extraction(token, &head, path);
  print_tokens(token);
  print_cmds(head);
  free_cmds(head);
  free_tokens(token);
  return (0);
}*/
