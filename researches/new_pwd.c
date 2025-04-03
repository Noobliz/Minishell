#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char *new_pwd(void)
{
	char *buffer = malloc(2);
	size_t	size = 2;
	if (!buffer)
		return (NULL);
	while (!getcwd(buffer, size))
	{
		free(buffer);
		size++;
		buffer = malloc(size);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

char *move_dir(char *dir_name)
{
	if (chdir(dir_name) == -1)
	{
		printf("error chdir\n");
		return (NULL);
	}
	return (new_pwd());
}

int main(int argc, char **argv)
{
  char *folder = "folder";
  char *path;
  
  if (argc > 1)
    folder = argv[1];
  path = move_dir(folder);
  if (!path)
    printf("error malloc\n");
  else
  { 
    printf("new pwd : %s", path);
    free(path);
  }
  return (0);
}
