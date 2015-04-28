#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

/*--------------------------------------DECLARATION METHODES------------------------------------*/
void* thread_prime_factors(void * u);

void print_prime_factors(uint64_t n);

/*--------------------------------------------METHODES-----------------------------------------*/

void* thread_prime_factors(void * u)
{
	//Déréférencement
	uint64_t* u2 = (uint64_t *) u;
	uint64_t n = *u2;
	print_prime_factors(n);
	
	return NULL;
}

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
	uint64_t nb2;
	FILE * file;
	file = fopen ("fileQuestion4pasEfficace.txt","r");
	char str[60];
	char str2[60];
	pthread_t thread0;
	pthread_t thread1;

	while ( fgets(str, 60, file)!=NULL && fgets(str2, 60, file)!=NULL )
	{
		nb=atol(str);
		nb2=atol(str2);
		printf("2 nb en même temps\n");
		//Attention en C l'appel des méthode est synchrone donc il faut d'abord créer un thread 
		//avant d'appeler des fonctions dans le main
		pthread_create(&thread0, NULL, thread_prime_factors, &nb);
		pthread_create(&thread1, NULL, thread_prime_factors, &nb2);

		

		//Wait for the thread0 to be done
		pthread_join(thread0, NULL);
		pthread_join(thread1, NULL);
	}


    return 0;
}
