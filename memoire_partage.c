#include "memoire_partage.h"

#include <stdio.h>		//gestion de l'affichage
#include <stdlib.h>		//gestion pour les mémoires
#include <string.h>		//gestion des chaînes de caractères
#include <sys/mman.h>	//gestion de la mémoire partagée "mmap"
#include <stdbool.h>	//gestion des Booléens

size_t Taille_memoire = TAILLE_MEMOIRE;
M_m memoire;


// initialisation de la mémoire partagée et du gap
void init_mem(void) {
	
	//si aucun fichier n'est attribué
	memoire.logs = (char*) mmap(NULL, Taille_memoire, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
	// memset(memoire.logs, 0, Taille_memoire);
	memoire.logs[0] ='\0';
	
	//Un gap dans une petite mémoire partagée
	memoire.top = (size_t*) mmap(NULL, sizeof(size_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
	size_t i = (size_t) 1;
	memcpy(memoire.top, &i, sizeof(size_t));
	
	//gestion de problème d'ouverture
	if (memoire.logs == MAP_FAILED || memoire.top == MAP_FAILED){
		perror("code 5 :l\'ouverture de la mémoire partagée a échouée\n");
		exit(5);
	}
}

//fonction pour enregistrer les message dans la mémoire partagée si possible
bool enregistrer(char* message){
	
	// taille du message en size_t
	size_t taille_du_message = strlen(message) * sizeof(char); //ici le \0 n'est pas pris en compte
	
	// vérifie la future taille de la memoire + la taille du message est <= à Taille_memoire
	if (*memoire.top + taille_du_message <= Taille_memoire){
		
		//ajout du message à la memoire partagée 
		strcat(memoire.logs, message);
		*memoire.top += taille_du_message;
		
		return false; //return pour condition
	}
	else {return true;}
}

// reset la mémoire et le gap
void reset_mem_and_affiche(char * en_plus){
	
	fprintf(stdout, "%s", memoire.logs);
	fprintf(stdout, "%s", en_plus);
	
	//remet tout les bits de la mémoire des message et le gap à 0
	// memset(memoire.logs, 0, Taille_memoire);
	memoire.logs[0] ='\0';
	
	size_t i = (size_t) 1;
	memcpy(memoire.top, &i, sizeof(size_t));
}

//clos proprement la mémoire
void close_mem(void){
	
	//fermeture de la mémoire partagée 
	if (munmap(memoire.logs, Taille_memoire) == -1 || munmap(memoire.top, sizeof(size_t)) == -1){
		perror("code 6 : Une erreur s\'est produite lors de la fermeture de mémoire partagée\n");
		exit(6);
	}
}

// gestion quand un message est reçu ou envoyé
void gestion_message(char* message){
	
	//son pour la console
	fprintf(stdout,"\a");
	fflush(stdout);
	
	if (enregistrer(message)){	//enregistre le message et renvoie un booléen
		reset_mem_and_affiche(message);
	}
}
