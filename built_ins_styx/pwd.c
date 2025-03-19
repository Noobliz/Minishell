#include "utils.h"

int	pwd(void)
{
	char	*str;

	str = get_env("1PWD");
	if (!str)
		return (-1);
	print(str);
	print("\n");
	free(str);
	return (0);
}

int main(void)
{
  pwd();
  return (0);
}
