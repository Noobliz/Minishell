#include "libbig.h"

//prints tokens
void print_tokens(t_token *tokens)
{
    printf("\n");
    while (tokens)
    {
        printf("Token: [%s] | Type: [%d]\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

//prints cmds
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
    if (cmd->built_in != -1)
      printf("is builtin\n");
    else
      printf("is not builtin\n");
    cmd = cmd->next;
  }
}
