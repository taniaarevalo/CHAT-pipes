#include "gestion_pipe.h"
#include "memoire_partage.h"


#include <sys/stat.h> //mkfifo

#include <stdlib.h> //exit()
#include <unistd.h> //close(), fwrite(), write(), fread(), read()
#include <fcntl.h> //open(), O_RDONLY, O_WRONLY
#include <string.h> //strlen(), memset(), strcpy(), strcat(), 
#include <errno.h> //EEXIST, errno
#include <stdio.h> //perror(), fflush(), stdin, printf(), fgets(), stdout, fprintf()





/* Fonction concaténant les pseudos dans une chaîne de caractères => nom du pipe */
char* creation_pipe(char *tmp, char *ecrivain, char *lecteur){

	//gestion du nom de pipe
	strcat(tmp, ecrivain);
	strcat(tmp, "-");
	strcat(tmp, lecteur);
	strcat(tmp, ".chat");

	/* Création du fichier pour le pipe nommé */
	if (mkfifo(tmp, PIPE_PERMISSION) == -1){
		if (errno != EEXIST){
			perror("code 7 : Erreur lors de la création du pipe nommé.");
			exit(7);
		}
	}

	return tmp;
}

/* fonction qui ouvre le pipe de lecture et lit ce qui a été envoyé */
void pipe_lecture(char* pipeDestUtil, char *pseudo, bool bot, bool manuel){

	//pour le gestion de signal
	bool vrai = true;

	// ouverture du pipe en mode lecture du pipe
	int des1 = open(pipeDestUtil, O_RDONLY);
		if (des1 == -1){
			perror("code 8 : Erreur avec le pipe d'écriture.");
			exit(8);
		}

	//initialisation
	char buffer[256];
	char tete[64]= "["; //48 mais 64 car puissance de 2
	char affiche[512] = ""; //512 selon le nombre d'octet atendus

	//creation de la tete des message
	if (bot){strcat(tete, pseudo); strcat(tete, "] ");}
	else{strcat(tete, "\x1B[4m"); strcat(tete, pseudo); strcat(tete, "\x1B[0m] ");}
	strcpy(affiche, tete);

	ssize_t b_read;


	while(vrai){
		b_read = read(des1, buffer, sizeof(buffer)-1);
		
		if (b_read < 0){
			perror("code 9 : Problème avec le read().");
			exit(9);
		}
		else if(b_read > 0){ 
			buffer[b_read] = '\0'; // en ajoutant ceci à la fin du message, il pourra s'afficher correctement
			
			strcat(affiche, buffer);

			if (manuel){gestion_message(affiche);}
			else {fprintf(stdout, "%s", affiche);}


			fflush(stdout);
			strcpy(affiche, tete);
			memset(buffer, 0, sizeof(buffer) -1);
		}
		else{
			break;
		}
	}
	close(des1);	//fermeture de des1
}


/* fonction qui ouvre le pipe d'écriture et écrit ce qui a été écrit dans le terminal */
void pipe_ecriture(char* pipeUtilDest, char *pseudo, bool bot, bool manuel){

	int des1 = open(pipeUtilDest, O_WRONLY); /* descripteur de fichier pour le pipe ouvert en mode écriture */
	if (des1 == -1){
		perror("code 8Erreur lors de l'ouvertude du pipe d'écriture.");
		exit(8);
		}

	char buffer[256];
	ssize_t b_write;
	while (fgets(buffer, sizeof(buffer), stdin)){
		if (!bot){
		fflush(stdin);
		printf("[\x1B[4m%s\x1B[0m] %s", pseudo, buffer);
		}
		else{
			fflush(stdin);
		}
		
		b_write = write(des1, buffer, strlen(buffer));
		if (b_write<0){
			perror("code 10 : Problème avec write().");
			exit(10);
		}
		else{
			memset(buffer, 0, sizeof(buffer)); // pk?
		}
		if (manuel){reset_mem_and_affiche("");}
	}
	close(des1);	//fermeture de des1
}
