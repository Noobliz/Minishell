#include "liball.h"
#include <stdio.h>

int main(void)
{
  t_env *env = env_item("NUM=see", 1);
  env->next = env_item("PATH=new", 1);
  char *str = copy("NUM+=here");
  if (!str)
    return (0);
  add_env(env, str);
  printf("here string :: %s\n", str);
  disp_env(env);
  free_env(env);
  free(str);
  return (0);
}
