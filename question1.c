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
	
    print_prime_factors(77); // expected result:   77: 7 11
    print_prime_factors(84); // expected result:   84: 2 2 3 7

    // expected result:   429496729675917: 3 18229 7853726291
    //print_prime_factors(429496729675917);


    return 0;
}
