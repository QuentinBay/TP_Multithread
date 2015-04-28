

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

uint64_t nb;
FILE * file;
char str[60];
pthread_t thread0;
pthread_t thread1;

//Gérer l'accès critique au file
pthread_mutex_t lock;

//Gérer l'accès critique au screen
pthread_mutex_t lockScreen;

//On code les nombres sur 64 bits donc au pire 64 facteurs (2^0*2¹...)
const int MAX_FACTORS=64;


void print_prime_factors(uint64_t n);
int get_prime_factors(uint64_t n,uint64_t*  dest);

void* thread_prime_factors(void * u)
{
	
	pthread_mutex_lock(&lock);
	
	while ( fgets(str, 60, file)!=NULL )
	{
		nb=atol(str);
		
		pthread_mutex_unlock(&lock);
	
		print_prime_factors(nb);
	
		pthread_mutex_lock(&lock);
	}
	
	pthread_mutex_unlock(&lock);
	
	return NULL;
}

void print_prime_factors(uint64_t n)
{
	uint64_t factors[MAX_FACTORS];
	int j,k;
	k=get_prime_factors(n,factors);
	
	pthread_mutex_lock(&lockScreen);
	

	printf("%ju: ",n);
	for(j=0; j<k; j++)
	{
		printf("%ju ",factors[j]);
	}
	printf("\n");
	
	pthread_mutex_unlock(&lockScreen);
}

int get_prime_factors(uint64_t n,uint64_t*  dest)
{
				/*****************
				* INITIALISATION *
				*****************/
	int compteur=0; //Garde le nombre de facteurs premiers que l'on rentre dans le tableau
	uint64_t i;


				/***************
				* TESTS POUR 2 *
				***************/
	while ( n%2 == 0)
	{
		n=n/2;
		dest[compteur]=(uint64_t)2;
		compteur++;
	}

				/***************
				* TESTS POUR 3 *
				***************/
	while ( n%3 == 0)
	{
		n=n/3;
		dest[compteur]=(uint64_t)3;
		compteur++;
	}

				/***************
				* TESTS POUR 5 *
				***************/
	while ( n%5 == 0)
	{
		n=n/5;
		dest[compteur]=(uint64_t)5;
		compteur++;
	}

		/************************************************************
		* TESTS POUR LE RESTE DES FACTEURS PREMIERS EN PARTANT DE 7 *
		************************************************************/
	for( i=7; n!=1 ; i++ )
	// On supprime les multiples de 2 et de 3 en incrementant alternativement
	// i de 4 et de 2
	{
		while (n%i==0)
		{
			// Tant que i est un facteur premier de n
			n=n/i;
			dest[compteur]=i;
			compteur++;
		}
	}
	
	if(n!=1)
	{
		dest[compteur]=n;
		compteur++;
	}

	return compteur;
}


int main(void)
{
	file = fopen ("fileQuestion4efficace.txt","r");
	
	if (pthread_mutex_init(&lock, NULL) != 0)
    {
	   printf("\n mutex file init failed\n");
       return 1;
    }

	if (pthread_mutex_init(&lockScreen, NULL) != 0)
    {
	   printf("\n mutex screen init failed\n");
       return 1;
    }

	//Attention en C l'appel des méthode est synchrone donc il faut d'abord créer un thread 
	//avant d'appeler des fonctions dans le main
	pthread_create(&thread0, NULL, thread_prime_factors, NULL);
	pthread_create(&thread1, NULL, thread_prime_factors, NULL);

	

	//Wait for the thread0 to be done
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);


	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&lockScreen);

    return 0;
}

