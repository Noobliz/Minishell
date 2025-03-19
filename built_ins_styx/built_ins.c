#include "utils.h"

int check_n(char *str)
{
  if (!str)
    return (0);
  if (str[0] != '-')
    return (0);
  if (str[1] != 'n')
    return (0);
  if (str[2] != '\0')
    return (0);
  return (1);
}

int echo(char **argv)
{
  int new_line;
  int i;

  i = 1;
  new_line = 1;
  if (!argv)
    return (0);
  if (check_n(argv[1]))
  {
    new_line = 0;
    i++;
  }
  while (argv[i])
  {
    print(argv[i]);
    i++;
    if (argv[i])
      print(" ");
  }
  if (new_line)
      print("\n");
  return (0);
}

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
    if (!is_alphanum(argv[i][j]) && !(argv[i][j] == '+' && argv[i][j +1] == '='))
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

int	pwd(void)
{
	char	*str;

	str = get_env("PWD");
	if (!str)
		return (-1);
	print(str);
	print("\n");
	free(str);
	return (0);
}

int built_int_att1(int func, char **argv, char **envp, t_env *env)
{
  if (envp && !env)
  {
    env = create_env(envp);
    if (!env)
      return (-1);
  }
  if (func == 0)
    return (echo(argv));
  if (func == 1)
    return (pwd());
  if (func == 2)
    return (export(argv, env));
  if (func == 3)
    return (unset(argv, env));
  if (func == 4)
  {
    disp_env(env);
    return (0);
  }
  return (0);
}
