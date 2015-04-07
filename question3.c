#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

void* thread_prime_factors(void * u)
{
	//Déréférencement
	uint64_t* u2 = (uint64_t *) u;
	uint64_t n = *u2;
	//print_prime_factors(n);
	printf("Calcul dans thread0 de : %ju \n",n);
	print_prime_factors(n);
	return NULL;
}

void print_prime_factors(uint64_t n)
{
					/*****************************
					*  		INITIALISATION		 *
					*****************************/
	

	printf("%ju : ", n );


	//On divise n par 2 jusqu'à ce qu'il ne soit plus divisible par 2
	while ( n%2 == 0)
	{
		n=n/2;
		printf("%d ", 2);
	}

	if ( n == 1 )
	{
		//On a trouvé tous les facteurs premiers
		printf("\n");
		return;
	}

			/***************************************************
			*  		RECHERCHE DES FACTEURS PREMIERS DE N 	   *
			****************************************************/
	uint64_t i, j;
	i=3;
	while(n!=1 && i <= n)
	{
		//printf("Boucle 1 :%ju\n", i);
		//Vérifions s'il s'agit d'un nombre premier
		for (j = 2; j <= i; j++)
		{
			//printf("Boucle 2 :%ju\n", j);
			if (i%j==0)
			{
				if (i==j)
				{
					//i est divisible par j et i = j donc c'est un nombre premier
					if (n%i==0)
					{
						//i est un facteur premier de n
						n=n/i;
						printf("%ju ", i);
						if (n==1)
						{
							//On a fini !
							printf("\n");
							return;
						}
						else
						{
							//On recommence à chercher un facteur premier
							i=1;
						}
						
					}
				}
				break;
			}
		}
		i+=2;
	}


}


int main(void)
{
	uint64_t nb;
	uint64_t nb2;
	FILE * file;
	file = fopen ("fileTest.txt","r");
	char str[60];
	char str2[60];
	pthread_t thread0;

	while ( fgets(str, 60, file)!=NULL && fgets(str2, 60, file)!=NULL )
	{
		nb=atol(str);
		nb2=atol(str2);
		printf("2 nb en même temps\n");
		print_prime_factors(nb);
		pthread_create(&thread0, NULL, thread_prime_factors, &nb2);

		//Wait for the thread0 to be done
		pthread_join(thread0, NULL);
	}


    return 0;
}
