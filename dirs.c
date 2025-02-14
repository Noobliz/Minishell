#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

//note : no error handling here, just a demo of the function's uses
int main(void)
{
  DIR *dirp;
  char *name = "folder"; //here the name of the folder you want to see
  struct dirent *content;
  
  dirp = opendir(name);
  content = readdir(dirp);
  while (content)
  {
    printf("next obj :: %s\n", content->d_name);
    content = readdir(dirp);
  }
  closedir(dirp);
}
