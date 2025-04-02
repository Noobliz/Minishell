

#include "../libbig.h"
#include <stdlib.h>

void	msg_error(t_data *data, char *str, int code)
{
	perror(str);
	data->last_exit_code = code;
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	stuff;
	size_t			i;

	ptr = s;
	stuff = c;
	i = 0;
	while (i < n)
	{
		ptr[i] = stuff;
		i++;
	}
	return (s);
}
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i])
		i++;
	j = 0;
	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}
