#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

//!!this program will appear to leak egregious amounts of memory (still reachable in valgrind) :: it does not, that's just readline being messy and not freeing its pointers before exit. that memory isn't lost, it's freed by the computer (program handler) as soon as the program closes.

//this demo is meant to prompt with "write here > ", then display "message received" unless the line received contains the word "bedazzle", in which case it transforms the line and displays it;
//if exit is given, the program closes;
//history available;

//note :: no demo of rl_redisplay here, will be in a different test file;

int is(char *cat, char *copy)
{
  int i = 0;
  
  if (!cat)
    return (0);
  while (cat[i] && cat[i] == copy[i])
    i++;
  if (cat[i] != copy[i])
    return (0);
  return (1);
}

int has(char *str, char *strstr)
{
  int i = 0;
  int j;
  
  while (str[i])
  {
    j = 0;
    while (strstr[j] && str[i + j] == strstr[j])
      j++;
    if (!strstr[j])
      return (1);
    i++;
  }
  return (0);
}

void  print(char *line)
{
  int i = 0;
  while (line[i])
  {
    write(1, &line[i], 1);
    i++;
  }
}

void  print_cap(char *line)
{
  int i = 0;
  char a;
  while (line[i])
  {
    a = line[i];
    if (line[i] != ' ' && (i == 0 || line[i - 1] == ' '))
      a = line[i] - 32;
    write(1, &a, 1);
    i++; 
  }
}

void  bedazzle(char *line)
{
  print("*.*.*.*.*.*");
  print_cap(line);
  print("*.*.*.*.*.*\n");
}

int main(void)
{
  char *line;
  char *prompt = "write here > ";
  
  line = readline(prompt);
  while (!is(line, "exit"))
  {
    if (line && !line[0])
    {
      free(line);
      line = NULL;
    }
    if (line)
    {
      if (has(line, "bedazzle"))
        bedazzle(line);
      else
        print("message received\n");
      rl_on_new_line();
      add_history(line);
      free(line);
      line = readline(prompt);
    }
  }
  free(line);
  rl_clear_history();
  return (0);
}
