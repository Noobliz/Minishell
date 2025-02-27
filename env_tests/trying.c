#include "libtest.h"

//same valgrind issue as readline.c, please disregard
//this echo only does echoing back into the terminal for now, but it will then expand to documents, including append and such. the "echoing" function will then be given an fd for easier access
//alright, this works ! rudimentary of course :: export does not yet compound variables, and the parsing is shaky at best, quotes don't work etc., but as long as you don't try to do something too complicated, it works fine !

void  disp_env(char **env)
{
  int i = 0;
  
  while (env && env[i])
  {
    print(env[i]);
    print("\n");
    i++;
  }
}

void  free_env(char **env, int end)
{
  int i;
  
  i = 0;
  if (end == -1)
  {
    while (env[i])
    {
      free(env[i]);
      i++;
    }
  }
  else
  {
    while (i < end)
    {
      free(env[i]);
      i++;
    }
  }
  free(env);
}

int unset(char *line, char ***new_env)
{
  int i = 0;
  char  **env_up;
  
  while ((*new_env)[i] && !is((*new_env)[i], line))
    i++;
  if (!(*new_env)[i])
    return (0);
  env_up = cccv(*new_env, NULL, -1, (*new_env)[i]);
  if (!env_up)
    return (-1);
  free_env(*new_env, -1);
  *new_env = env_up;
  return (0);
}

int exporting(char *line, char ***new_env)
{
  char  *new_line = copy_str(line);
  char  **env_up;
  
  if (!new_line)
    return (-1);
  env_up = cccv(*new_env, new_line, 1, NULL);
  if (!env_up)
    return (-1);
  free_env(*new_env, -1);
  *new_env = env_up;
  return (0);
}

char  *get_env(char *var, char **new_env, int len)
{
  int i;
  
  i = 0;
  while (new_env[i] && !is(new_env[i], var))
    i++;
  if (new_env[i])
    return (&new_env[i][len + 1]);
  return (NULL);
}

int  print_var(char *var, char **new_env)
{
  char  *env_var;
  int i;

  i = 0;
  while (var[i] && var[i] != ' ')
    i++;
  env_var = get_env(var, new_env, i);
  if (!env_var)
  {
    print("$");
    return (0);
  }
  print(env_var);
  return (i);
}

void  echoing(char *line, char **new_env)
{
  int i = 0;
  
  while (line[i] == ' ')
    i++;
  while (line[i])
  {
    if (line[i] == '$' && line[i + 1])
      i = i + print_var(&line[i + 1], new_env);
    else
      write(1, &line[i], 1);
    i++;
  }
  print("\n");
}

void  print(char *str)
{
  int i = 0;
  
  while (str && str[i])
  {
    write(1, &str[i], 1);
    i++;
  }
}

int is(char *line, char *find)
{
  int i;
  
  i = 0;
  while (find[i] && find[i] != ' ' && find[i] == line[i])
    i++;
  if (find[i] && find[i] != ' ')
    return (0);
  return (1);
}

char  *copy_str(char *str)
{
  int i;
  char *s;
  
  i = 0;
  while (str[i])
    i++;
  s = malloc(i + 1);
  if (!s)
    return (NULL);
  i = 0;
  while (str[i])
  {
    s[i] = str[i];
    i++;
  }
  s[i] = '\0';
  return (s);
}

char **cccv(char **env, char *var, int add, char *del)
{
  char  **new_env;
  int i;
  int j;

  i = 0;
  while (env[i])
    i++;
  new_env = malloc(sizeof(char *) * (i + add + 1));
  add = add + i;
  i = 0;
  j = 0;
  while (env[j] && i < add)
  {
    if (del && env[j] == del)
      j++;
    new_env[i] = copy_str(env[j]);
    if (!new_env[i])
    {
      free_env(new_env, i);
      return (NULL);
    }
    i++;
    j++;
  }
  if (var)
  {
    new_env[i] = var;
    i++;
  }
  new_env[i] = NULL;
  return (new_env);
}

int main(int argc, char **argv, char **env)
{
  char **new_env;
  if (argc > 1)
    return (0);
  if (argv[1])
    return (0);
  new_env = cccv(env, NULL, 0, NULL);
  if (!new_env)
    return (0);
  char *line = readline("ask here > ");
  while (line && !is(line, "quit"))
  {
    if (is(line, "echo"))
      echoing(&line[4], new_env);
    else if (is(line, "unset"))
    {
      if (unset(&line[5], &new_env) == -1)
        break ;
    }
    else if (is(line, "export"))
    {
      if (exporting(&line[7], &new_env) == -1)
        break ;
    }
    else if (is(line, "env"))
      disp_env(new_env);
    add_history(line);
    free(line);
    rl_on_new_line();
    line = readline("ask here > ");
  }
  free_env(new_env, -1);
  if (line)
    free(line);
  rl_clear_history();
  return (0);
}
