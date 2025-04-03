#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_tty
{
  char *name;
  int slot;
}              t_tty;

t_tty	*tty_info(t_tty *tty, int fd)
{
	if (isatty(fd) == 0)
		return (NULL);
	tty->name = ttyname(fd);
	tty->slot = ttyslot();
	return (tty);
}

int main(void)
{
  t_tty *str;
  t_tty *tmp;
  int fd = dup2(2, 9);
  
  str = (t_tty *)malloc(sizeof(t_tty));
  if (!str)
  {
    printf("no struct(malloc)\n");
    return (0);
  }
  tmp = tty_info(str, fd);
  if (!tmp)
    printf("isn't a tty\n");
  else
    printf("the fd %d, the open slot %d and the tty connected to that fd, named %s\n", fd, str->slot, str->name);
  free(str);
  return (0);
}
