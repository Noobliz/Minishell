#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	printf("here the path var :: %s\n", getenv("PATH"));
	printf("here the var var :: %s\n", getenv("VAR"));
	return (0);
}
