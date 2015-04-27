#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

void print_prime_factors(uint64_t n)
{
	printf("%ju : ", n );

	uint64_t i;

	for( i=2; n!=1 ; i++ )
	{
		while (n%i==0)
		{
			// Tant que i est un facteur premier de n
			n=n/i;
			printf("%ju ", i);
		}
	}
	//On a fini !
	printf("\n");

	return;
}


int main(void)
{
	uint64_t nb;
	FILE * file;
	file = fopen ("fileQuestion2.txt","r");
	char str[60];
	while ( fgets(str, 60, file)!=NULL)
	{
		nb=atol(str);
		print_prime_factors(nb);
		
	}


    return 0;
}
