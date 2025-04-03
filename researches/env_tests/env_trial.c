#include "libtest.h"

//compile with trying for testing (don't forget to put the original main in comments)
//this program displays the environment :: first as is, copied from the og env, then with the added argv[1], then with it deleted ((meant to replicate a export/unset situation))

int main(int argc, char **argv, char **env)
{
  char **new_env;
  char **tmp;
  char *str;

  if (argc != 2)
    return (0);
  new_env = cccv(env, NULL, 0, NULL);
  if (!new_env)
    return (0);
  disp_env(new_env);
  str = copy_str(argv[1]);
  tmp = cccv(new_env, str, 1, NULL);
  free_env(new_env, -1);
  if (!tmp)
    return (0);
  new_env = tmp;
  print("\n\n\n\n\n\n");
  disp_env(new_env);
  tmp = cccv(new_env, NULL, -1, str);
  free_env(new_env, -1);
  if (!tmp)
    return (0);
  new_env = tmp;
  print("\n\n\n\n\n\n");
  disp_env(new_env);
  free_env(new_env, -1);
  return (0);
}
