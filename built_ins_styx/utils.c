#include "utils.h"

void  print(char *str)
{
  int i;

  i = 0;
  while (str && str[i])
  {
    write(1, &str[i], 1);
    i++;
  }
}

int is_alpha(int a)
{
  if (a >= 'a' && a <= 'z')
    return (1);
  if (a >= 'A' && a <= 'Z')
    return (1);
  if (a == '_')
    return (1);
  return (0);
}

int is_alphanum(int a)
{
  if (is_alpha(a))
    return (1);
  if (a >= '0' && a <= '9')
    return (1);
  if (a == '_')
    return (1);
  return (0);
}

void	disp_env(t_env *env)
{
	while (env)
	{
		print(env->var);
		env = env->next;
		print("\n");
	}
}
