#include "gestion_parametre.h"

#include <stdio.h> //fprintf, stderr
#include <string.h> //strlen, strcmp
#include <stdlib.h> //exit()
#include <stdbool.h> //bool

//variable globale
Param parametre;

/* Regarde si un des opérateurs de op est présent dans les pseudos, retourne 1 si oui */
bool contient_operateur(char *pseudo){
	const char op[] = {'/', '-', '[', ']'};
	char* ptr_lettre = pseudo;

	for (unsigned long int a = 0; a < strlen(pseudo); ++a){
		ptr_lettre = pseudo + a;
		for (unsigned long int i = 0; i < sizeof(op); ++i){
		if (*ptr_lettre == op[i]){return 1;}
		}
	}
	return 0;
}

/* Fonction pour voir si un des pseudos n'est pas "." ou "..". */
bool pseudo_point(char *pseudo){
	if ((strcmp(pseudo, ".") == 0 )||(strcmp(pseudo, "..") == 0 )){
		return 1;
	}
	return 0;
}

/* Vérifie que les patramètres sont justes */
Param gestion_params(int argc, char* argv[]){

	/* condition 1: pas assez d'arguments */
	if (argc < 3){
		fprintf(stderr, "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]");
		exit(1);
	}

	Param parametre = {argv[1], argv[2], false, false};

	/* condition 2: longueur des pseudos doit être plus courte que 30 caractères */
	if (strlen(parametre.pseudo_utilisateur) > TAILLE_PSEUDOS || strlen(parametre.pseudo_destinataire) > TAILLE_PSEUDOS){
		fprintf(stderr, "Fin du programme car la longueur du/des pseudonyme(s) dépasse(nt) les 30 caractères.\n"); 
		exit(2);
	}

	/* condition 3 : pseudos ne contiennent pas de symboles de /, [, ], - */
	if ((contient_operateur(parametre.pseudo_utilisateur) == 1) || (contient_operateur(parametre.pseudo_destinataire) == 1)){
		fprintf(stderr, "Fin du programme car le(s) pseudo(s) contient/nent un/des opérateur(s) suivant(s): /, -, [, ].\n");
		exit(3);
	}

	/* condition 4 : pseudos ne sont pas "." ou ".." */
	if ((pseudo_point(parametre.pseudo_utilisateur) == 1) || (pseudo_point(parametre.pseudo_destinataire) == 1)){
		fprintf(stderr, "Fin du programme car le(s) pseudo(s) ne peut pas être [.] ou [..].\n");
		exit(3);
	}

	/* condition 5 : si nous avons des arguments --bot, --manuel */
	for (int i = 3; i < argc; ++i){
		if (strcmp(argv[i], "--bot") == 0){	parametre.bot = true;}
		else if(strcmp(argv[i], "--manuel") == 0){parametre.manuel = true;}
	}

	return parametre;
}
