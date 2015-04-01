#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

/*
void * bidon (void *)
{
	FILE * file;
	file = fopen ("fileTest.txt","r");
	char str[60];
	while ( fgets(str, 60, file)!=NULL)
	{
		nb=atol(str);
		print_prime_factors(nb);
	}
}*/


void* print_prime_factors(void* u)
{
	uint64_t *u2 = (uint64_t*)u;
	uint64_t n = *u2;
	//Boucle pour parcourir les nombres premiers
	uint64_t nbTest;
	uint64_t diviseur;
	uint64_t nbPremiers[n];
	uint64_t compteur=0;

/*----------------------------------Trouver les nombres premiers--------------------------------*/
	for (nbTest = 2; nbTest < n; nbTest++)
	{
		//printf("Boucle 1 :%ju\n", nbTest);
		diviseur = 2;
		for(diviseur = 2; diviseur <= nbTest; diviseur++)
		{
			//printf("Boucle 2 :%ju\n", diviseur);
			if (nbTest%diviseur==0)
			{
				//On a trouve un diviseur
				if (nbTest==diviseur)
				{
					// c est un nombre premier 
					nbPremiers[compteur]=nbTest;
					compteur++;
					//printf("%ju\n", nbTest);
				}
				else
				{
					break;
				}

			}
		}
	}
    // your code goes here: print "n", a colon, and then the prime factors

/*-----------------------------------------Decomposer n----------------------------------------*/
	printf("%ju : ", n);
	uint64_t i;
	while(n!=1)
	{
		for (i = 0; i < compteur; ++i)
		{
			/* testons si n est divisible par les nbPremiers */
			if (n%nbPremiers[i]==0)
			{
				/* on a trouve un nombre premier de n */
				printf("%ju ", nbPremiers[i]);
				n=n/nbPremiers[i];
				break;
			}
		}
	}
	printf("\n");
}

int main(void)
{
	uint64_t nb;
	uint64_t nb2;
	FILE * file;
	file = fopen ("fileTest.txt","r");
	char str[60];
	char str2[60];
	//Pour les threads
	int thread;
	pthread_t unThread;
	while ( fgets(str, 60, file)!=NULL && fgets(str2, 60, file)!=NULL)
	{
		nb=atol(str);
		nb2=atol(str2);
		thread = pthread_create(&unThread, NULL, print_prime_factors, &nb2);
		thread = pthread_join(unThread);
	}
    //print_prime_factors(77); // expected result:   77: 7 11
    //print_prime_factors(84); // expected result:   84: 2 2 3 7
    //print_prime_factors(429496);
    // expected result:   429496729675917: 3 18229 7853726291
    //print_prime_factors(429496729675917);


    return 0;
}
