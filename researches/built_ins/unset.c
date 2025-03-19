#include "utils.h"

int unset(char **argv, t_env *env)
{
  int i;

  if (!env)
    return (0);
  i = 1;
  while (argv[i])
  {
    if (argv[i][0] != '?')
      del_env(env, argv[i]);
    i++;
  }
  return (0);
}
