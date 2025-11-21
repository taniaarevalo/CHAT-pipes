#include "gestion_pipe.h"
#include "gestion_parametre.h"
#include "gestion_signal.h"
#include "memoire_partage.h"

#include <stdbool.h>


#include <sys/wait.h>

#include <sys/types.h>
#include <unistd.h> //fork(), unlink()

#include <stdlib.h>
#include <stdio.h> //perror()


int main(int argc, char* argv[]){

	Param par = gestion_params(argc, argv);

	/* création du nom du pipe */
	char tmp1[71] = "/tmp/", tmp2[71] = "/tmp/";
	char *pipeUtilDest = creation_pipe(tmp1, par.pseudo_utilisateur, par.pseudo_destinataire);
	char *pipeDestUtil = creation_pipe(tmp2, par.pseudo_destinataire, par.pseudo_utilisateur);


	//initialise le gestionaire de signal
	init_sig_obj(&par.manuel, pipeUtilDest, pipeDestUtil);
	
	//initialisation de la mémoire partagée
	if (par.manuel){init_mem();}
	

	pid_t pid = fork();

	/* vérifier que tout va bien */
	if (pid == -1){perror("code 4 :erreur avec fork"); return 4;}

	/* processus fils */
	if (pid == 0){
		captation_de_signal(pid);
		pipe_lecture(pipeDestUtil, par.pseudo_destinataire, par.bot, par.manuel);
	}

	/* processus père/originel */
	else {
		captation_de_signal(pid);
		pipe_ecriture(pipeUtilDest, par.pseudo_utilisateur, par.bot, par.manuel);
		wait(NULL);
		
	}

	//fermeture de la mémoire partagée
	if (par.manuel){close_mem();}
	
	unlink(pipeDestUtil);
	unlink(pipeUtilDest);
	return 0;
}
