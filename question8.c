

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
	
	pthread_mutex_lock(&lock);
	
	if (k ==-1)
	{
		printf("ERROR : fonction get_prime_factors");
		return;
	}
	printf("%ju: ",n);
	for(j=0; j<k; j++)
	{
		printf("%ju ",factors[j]);
	}
	printf("\n");
	
	pthread_mutex_unlock(&lock);
}

int get_prime_factors(uint64_t n,uint64_t*  dest)
{
					/*****************************
					*  		INITIALISATION		 *
					*****************************/
	//Garder le nombre de facteurs premiers que l'on rentre dans le tableau
	int compteur=0;
	
	//On divise n par 2 jusqu'à ce qu'il ne soit plus divisible par 2
	while ( n%2 == 0)
	{
		n=n/2;
		dest[compteur]=(uint64_t)2;
		compteur++;
		
	}

	if ( n == 1 )
	{
		//On a trouvé tous les facteurs premiers
		printf("\n");
		return compteur;
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
						dest[compteur]=i;
						compteur++;
						
						if (n==1)
						{
							return compteur;
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
	return -1;
}


int main(void)
{
	file = fopen ("fileQuestion4.txt","r");
	
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

