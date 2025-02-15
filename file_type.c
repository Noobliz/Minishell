#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

//this program takes a name into account and finds out if the obj by this name is a dir or not
//error handling here, including whether obj exists or not.

int main(int argc, char **argv)
{
  char *dir_name = ".";
  struct stat statbuf;
  
  if (argc > 1)
    dir_name = argv[1];
  if (access(dir_name, F_OK) == -1)
  {
    printf("%s not found\n", dir_name);
    return (0);
  }
  if (lstat(dir_name, &statbuf) == -1)
  {
    printf("error stat");
    return (0);
  }
  if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
    printf("%s is a directory !\n", dir_name);
  else
    printf("%s is not a directory\n", dir_name);
  return (0);
}
