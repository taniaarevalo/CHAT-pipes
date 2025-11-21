#ifndef MEMOIRE_PARTAGE_H
#define MEMOIRE_PARTAGE_H

#include <stddef.h>		//pour n'avoir que la definition de size_t

#define TAILLE_MEMOIRE 4096

//structure pour la mémoire partagée
typedef struct memoire_message {
	char* logs;			//pointeur pour mémoire partagée des messages
	size_t* top;		//pointeur pour mémoire partagée des taille de l'allocation de mémoire
} M_m;

void init_mem(void);
void close_mem(void);
void reset_mem_and_affiche(char * en_plus);
void gestion_message(char* message);


#endif
