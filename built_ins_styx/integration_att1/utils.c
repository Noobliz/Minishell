#include "libbig.h"

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

char  *join(char *s, char *s2)
{
  int len;
  int i;
  int j;
  char  *str;

  if (!s && !s2)
  	return (NULL);
  len = len_str(s);
  len = len + len_str(s2);
  str = malloc(len + 1);
  if (!str)
    return (NULL);
  i = 0;
  while (s && s[i])
  {
    str[i] = s[i];
    i++;
  }
  j = 0;
  while (s2 && s2[j])
  {
    str[i + j] = s2[j];
    j++;
  }
  str[i + j] = '\0';
  return (str);
}

int len_str(char *str)
{
  int i;

  i = 0;
  while (str && str[i])
    i++;
  return (i);
}
