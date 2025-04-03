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
