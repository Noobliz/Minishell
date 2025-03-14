#include "utils.h"

void  print_exp_syntax(char *str)
{
  print("bash: export: `");
  print(str);
  print("': not a valid identifier\n");
}

int one_line_export(char **argv, int i, t_env *env)
{
  int j;

  if (!argv[i])
    return (0);
  if (!is_alpha(argv[i][0]))
  {
    print_exp_syntax(argv[i]);
    return (one_line_export(argv, i + 1, env));
  }
  j = 1;
  while (argv[i][j] && argv[i][j] != '=')
  {
    if (!is_alphanum(argv[i][j]))
    {
      print_exp_syntax(argv[i]);
      return (one_line_export(argv, i + 1, env));
    }
    j++;
  }
  if (argv[i][j] == '=' && env
    && add_env(env, argv[i]) == -1)
    return (-1);
  return (one_line_export(argv, i + 1, env));
}

int exportt(char **argv, t_env *env)
{
  if (!argv)
    return (0);
  if (!argv[1])
    disp_env(env);
  return (one_line_export(argv, 1, env));
}

/*int main(int argc, char **argv, char **envp)
{
  t_env *env = create_env(envp);
  if (!env)
    return (0);
  if (argc && exportt(argv, env) == -1)
    return (-1);
  disp_env(env);
  free_env(env);
  return (0);
}*/
