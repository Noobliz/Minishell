#include "utils.h"

char	*new_pwd(void)
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

int	pwd(void)
{
	char	*str;

	str = new_pwd();
	if (!str)
		return (-1);
	print(str);
	print("\n");
	free(str);
	return (0);
}
