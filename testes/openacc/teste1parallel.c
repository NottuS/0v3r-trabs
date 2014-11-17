#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	/* code */

	int v[1000];
	int i;

	#pragma acc kernels
	for (i = 0; i < 1000; ++i)
	{
		v[i] = i;
	}

	for (i = 0; i < 1000; ++i)
	{
		printf("%d ", v[i]);
	}
	printf("\n");
	return 0;
}