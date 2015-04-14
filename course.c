#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

/*-------------------------------------------ATTRIBUTS------------------------------------------*/

uint64_t nb;
FILE * file;
char str[60];
pthread_t thread0;
pthread_t thread1;

//Gérer l'accès critique au file
pthread_mutex_t lockFile = PTHREAD_MUTEX_INITIALIZER;

//Gérer l'accès critique au screen
pthread_mutex_t lockScreen = PTHREAD_MUTEX_INITIALIZER;

//On code les nombres sur 64 bits donc au pire 64 facteurs (2^0*2¹...)
const int MAX_FACTORS=64;


////////////////////////////////////////////////////////////////////////// ATTRIBUTS ARBRE BINAIRE

typedef struct node
{
    uint64_t key; //n
    uint64_t nbFactors; //Nombre de facteurs premiers
    uint64_t *factorsTree; //Tableau des facteurs premiers de n.
    struct node *left;
    struct node *right;
} node ;

 node *arbre = NULL;
 
 //Gérer l'accès critique à l'arbre
pthread_mutex_t lockTree = PTHREAD_MUTEX_INITIALIZER;

/*--------------------------------------DECLARATION METHODES------------------------------------*/
void* thread_prime_factors(void * u);

void print_prime_factors(uint64_t n);

int get_prime_factors(uint64_t n,uint64_t*  dest);

int get_prime_factors2(uint64_t n,uint64_t*  dest);

void addNode (node** tree, uint64_t unNombre, uint64_t unNbFacteurs , uint64_t * fateurs);

node * searchNode (uint64_t uneCle);

void displayTree (node* tree);

void clearTree(node** tree);

/*--------------------------------------------METHODES-----------------------------------------*/

void* thread_prime_factors(void * u)
// Algo : Prend le jeton d'acces au fichier, lit une ligne, rend le jeton 
// et appelle "print_prime_factors" avec le nombre lu sur le fichier.
{
	
	pthread_mutex_lock(&lockFile);
	
	while ( fgets(str, 60, file)!=NULL )
	{
		nb=atol(str);
		
		pthread_mutex_unlock(&lockFile);
	
		print_prime_factors(nb);
	
		pthread_mutex_lock(&lockFile);
	}
	
	pthread_mutex_unlock(&lockFile);
	
	return NULL;
}



void print_prime_factors(uint64_t n)
// Algo : Appelle "get_prime_factors", prend le jeton pour l'acces a l'ecran,
// affiche la liste des facteurs premiers de n, et rend le jeton de l'ecran.
{
	uint64_t * resTab=NULL;
	
	int j,nbPremiers;

	//Verfions que l'on a pas deja fait le calcul!
	pthread_mutex_lock(&lockTree);
	node * noeudCalcule=searchNode(n);
	pthread_mutex_unlock(&lockTree);

	if (noeudCalcule!=NULL)
	{
		// n a deja ete calcule avant
		nbPremiers=noeudCalcule->nbFactors;
		resTab=noeudCalcule->factorsTree;
	}
	else
	{
		// n n'a pas encore ete calcule
		uint64_t factors[MAX_FACTORS];
		nbPremiers=get_prime_factors(n,factors);

		if (nbPremiers ==-1)
		{
			printf("ERROR : fonction get_prime_factors");
			return;
		}
		resTab=factors;

		//Gardons en memoire ce calcul !
		pthread_mutex_lock(&lockTree);
		addNode(&arbre, n, nbPremiers, resTab);
		pthread_mutex_unlock(&lockTree);
		//printf("arbre->factorsTree : %ju\n",arbre->factorsTree[0]);
	}
	
	//Affichage du resultat
	pthread_mutex_lock(&lockScreen);
	
	printf("%ju: ",n);
	for(j=0; j<nbPremiers; j++)
	{
		printf("%ju ",resTab[j]);
	}
	printf("\n");

	pthread_mutex_unlock(&lockScreen);
}

int get_prime_factors(uint64_t n, uint64_t*  dest)
{
				/*****************
				* INITIALISATION *
				*****************/
	int compteur=0; //Garde le nombre de facteurs premiers que l'on rentre dans le tableau
	int prime=1; //Savoir si on a un nombre premier : 1=non, 0=oui
	uint64_t i,j;
	int sommet=1; // Indice de la position dans le tableau
	uint64_t essai;
	uint64_t pasI=4;
	printf("Appel de get_prime_factors\n");
	uint64_t dp[(uint64_t)(n/3)];
	printf("Creation du tableau !\n");
	dp[0]=(uint64_t)5;

				/***************
				* TESTS POUR 2 *
				***************/
	while ( n%2 == 0)
	{
		n=n/2;
		dest[compteur]=(uint64_t)2;
		compteur++;
		//Verfions que l'on a pas deja fait le calcul!
		pthread_mutex_lock(&lockTree);
		node * noeudCalcule=searchNode(n);
		pthread_mutex_unlock(&lockTree);

		if (noeudCalcule!=NULL)
		{
			// Trouve !
			//uint64_t * tmp = *noeudCalcule->factorsTree;
			int l;
			for (l = 0; l < noeudCalcule->nbFactors; ++l)
			{
				dest[compteur]=noeudCalcule->factorsTree[l];
				compteur++;
			}
			return compteur;
		}
	}

				/***************
				* TESTS POUR 3 *
				***************/


	while ( n%3 == 0)
	{
		n=n/3;
		dest[compteur]=(uint64_t)3;
		compteur++;
		//Verfions que l'on a pas deja fait le calcul!
		pthread_mutex_lock(&lockTree);
		node * noeudCalcule=searchNode(n);
		pthread_mutex_unlock(&lockTree);

		if (noeudCalcule!=NULL)
		{
			// Trouve !
			//uint64_t * tmp = *noeudCalcule->factorsTree;
			int l;
			for (l = 0; l < noeudCalcule->nbFactors; ++l)
			{
				dest[compteur]=noeudCalcule->factorsTree[l];
				compteur++;
			}
			return compteur;
		}
	}


		/************************************************************
		* TESTS POUR LE RESTE DES FACTEURS PREMIERS EN PARTANT DE 5 *
		************************************************************/

	for( i=7; (n!=1)&&(i<=n) ; i+=pasI,pasI=(uint64_t)6-pasI )
	// On supprime les multiples de 2 et de 3 en incrementant alternativement
	// i de 4 et de 2
	{
		prime=0;
		for( essai=dp[j=0] ; (i%essai)&&(essai*essai<i) ; essai=dp[++j] )
		// On incremente j de 1 pour se deplacer dans le tableau dp des nombres
		// premiers que l'on teste successivement
		{
			if(i%essai==0) prime=1;
		}
		if(prime==0)
		{
			//on a trouve un nombre premier
			dp[sommet]=i;
			sommet++;

			while (n%i==0)
			{
				// Tant que i est un facteur premier de n
				n=n/i;
				dest[compteur]=i;
				compteur++;
				
				if (n==1)
				{
					return compteur;
				}
				else
				{
					//Verfions que l'on a pas deja fait le calcul!
					pthread_mutex_lock(&lockTree);
					node * noeudCalcule=searchNode(n);
					pthread_mutex_unlock(&lockTree);
					if (noeudCalcule!=NULL)
					{
						// Trouve !
						//uint64_t * tmp = *noeudCalcule->factorsTree;
						int l;
						for (l = 0; l < noeudCalcule->nbFactors; ++l)
						{
							dest[compteur]=noeudCalcule->factorsTree[l];
							compteur++;
						}
						return compteur;
					}
				}
			}
		}	
	}
	return compteur;
}

//////////////////////////////////////////////////////////////////////// METHODES ARBRE BINAIRE 

void addNode (node** tree, uint64_t unNombre, uint64_t unNbFacteurs , uint64_t * facteurs)
// Algo : Cree un noeud, parcours l'arbre et ajoute le noeud.
{
				/*****************************
				*  		INITIALISATION		 *
				*****************************/
	printf("Appel de addNode avec : %ju !\n", facteurs[0]);
	node *previous=NULL;
	node *current=*tree;
	
	node * unNoeud=(node*)malloc(sizeof(node));
	unNoeud->key=unNombre;
	unNoeud->nbFactors=unNbFacteurs;
	// Allouons un espace memoire pour stocker les valeurs du tableau sinon elles seront perdus lors
	// du changement de contexte de print_prime_factors
	unNoeud->factorsTree=(uint64_t*)malloc(sizeof(facteurs));
	int i;
	for (i = 0; i < unNbFacteurs; ++i)
	{
		unNoeud->factorsTree[i]=facteurs[i];
	}
	
	unNoeud->left=NULL;
	unNoeud->right=NULL;

		/*******************************************
		* REHCERHCHE DE LA POSITION DU FUTUR NOEUD *
		*******************************************/	
	if (current==NULL)
	{
		//Arbre vide
		*tree=unNoeud;
	}
	else
	{
		while(current != NULL)
		{
			//Tant qu'on n'a pas atteint une feuille
			previous=current;
			
			if(unNoeud->key < current->key)
			{
				//On se déplace à gauche
				current=current->left;
				if ( current== NULL )
				{
					//On ajoute le nouveau noeud
					previous->left=unNoeud;
					break;
				}
			}
			else if (unNoeud->key > current->key)
			{
				//On doit se déplacer sur la branche droite
				current=current->right;
				if ( current== NULL )
				{
					//On ajoute le nouveau noeud
					previous->right=unNoeud;
					break;
				}
			}
			else
			{
				//La valeur est déjà dans l'arbre
				break;
			}
		}
	}
	
}

node * searchNode (uint64_t uneCle)
{
	node *current=arbre;
	
	if (current==NULL)
	{
		//Arbre vide
		return NULL;
	}
	
	while(current != NULL)
	{
		if(uneCle < current->key)
		{
			//On se déplace à gauche
			current=current->left;
			if ( current== NULL )
			{
				return NULL;
			}
		}
		else if (uneCle > current->key)
		{
			//On doit se déplacer sur la branche droite
			current=current->right;
			if ( current== NULL )
			{
				return NULL;
			}
		}
		else
		{
			//On a trouve le noeud correspondant
			return current;
		}
	}
	return NULL;
}

void displayTree (node* tree)
// Algo : Parcours de l'arbre en utilisant une fonction récursive, et en commencant
// par les valeurs plus petites (gauche).
{
	//Arbre vide
	printf("Appel de displayTree !\n");
    if(tree==NULL)
    {
    	printf("Arbre vide !\n");
    	return;
    }

    //On commence par les valeurs les plus petites
    if(tree->left!=NULL)  displayTree(tree->left);

    printf("key : %ju;\n", tree->key);

    //On affiche ensuite les valeurs plus grandes
    if(tree->right!=NULL) displayTree(tree->right);
}

void clearTree(node** tree)
// Algo : Parcours de l'arbre, destruction de tous les noeuds que l'on a cree
// avec un appel recursif a la fonction
{
	node * current = *tree;

	//Arbre deja vide
	if (current==NULL) return;

	//Descendons aux feuilles de l'arbre
	if(current->left!=NULL) clearTree(&current->left);
	if(current->right!=NULL) clearTree(&current->right);

	//On a atteint une feuille, detruisons la !
	free(current->factorsTree);
	free(current);
	*tree=NULL;
}

int main(void)
{
	file = fopen ("fileQuestion4.txt","r");

	//Attention en C l'appel des méthode est synchrone donc il faut d'abord créer un thread 
	//avant d'appeler des fonctions dans le main
	pthread_create(&thread0, NULL, thread_prime_factors, NULL);
	pthread_create(&thread1, NULL, thread_prime_factors, NULL);

	

	//Wait for the thread0 and the thread1 to be done
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);


	pthread_mutex_destroy(&lockTree);
	pthread_mutex_destroy(&lockFile);
	pthread_mutex_destroy(&lockScreen);
	
	//Liberons la memoire !
	printf("DESTRUCTION DE L'ARBRE :\n");
	clearTree(&arbre);
	displayTree(arbre);
    return 0;
}


