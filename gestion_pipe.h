#ifndef GESTION_PIPE
#define GESTION_PIPE

/* LIBRAIRIES UTILISÉES et scripts*/
#include <stdbool.h> //bool

/* CONSTANTES */

#define PIPE_PERMISSION 0666


/* PROTOTYPES DE FONCTIONS */


char* creation_pipe(char *tmp, char *ecrivain, char *lecteur);
void pipe_lecture(char* pipeDestUtil, char *pseudo, bool bot, bool manuel);
void pipe_ecriture(char* pipeUtilDest, char *pseudo, bool bot, bool manuel);

#endif
