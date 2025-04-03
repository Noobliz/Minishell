#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

//this takes the name of a folder as argument and lists out its contents. please don't break, no err handling. just demo

int main(int argc, char **argv)
{
  DIR *dirp;
  char *name = "folder";
  struct dirent *content;
  
  if (argc > 1)
    name = argv[1];
  dirp = opendir(name);
  content = readdir(dirp);
  while (content)
  {
    printf("next obj :: %s\n", content->d_name);
    content = readdir(dirp);
  }
  closedir(dirp);
}
