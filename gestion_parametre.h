#ifndef GESTION_PARAMETRE_H
#define GESTION_PARAMETRE_H

#include <stdbool.h> //bool

#define TAILLE_PSEUDOS 30

typedef struct parametre {
	char* pseudo_utilisateur;		//pointeur pour la mémoire partagée
	char* pseudo_destinataire;		//pointeur pour la mémoire partagée des taille de l'allocation de mémoire
	bool bot;						//booléen pour le mode bot
	bool manuel;					//booléen pour le mode manuel
} Param;

Param gestion_params(int argc, char* argv[]);

#endif
