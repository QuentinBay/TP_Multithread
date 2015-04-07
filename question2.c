#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

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

void print_prime_factorsVersionNulle(uint64_t n)
/*
*Cette méthode ne fonctionne pas sur des nombres > 5.10^5, elle renvoie une segmentation fault*/
{
	//Boucle pour parcourir les nombres premiers
	uint64_t nbTest;
	uint64_t diviseur;
	uint64_t nbPremiers[n];
	uint64_t compteur=0;

/*----------------------------------Trouver les nombres premiers--------------------------------*/
	for (nbTest = 2; nbTest < n; nbTest++)
	{
		//
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
	FILE * file;
	file = fopen ("fileQuestion4.txt","r");
	char str[60];
	while ( fgets(str, 60, file)!=NULL)
	{
		nb=atol(str);
		print_prime_factors(nb);
		
	}


    return 0;
}
