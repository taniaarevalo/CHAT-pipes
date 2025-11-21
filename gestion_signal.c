#include "gestion_signal.h"
#include "memoire_partage.h"
#include "gestion_pipe.h"


#include <sys/types.h>
#include <unistd.h> //unlink(), 

#include <signal.h>
#include <stdbool.h>
#include <stdio.h> //printf()
#include <stdlib.h> //exit()


Cqif objet;

void init_sig_obj(bool* manuel, char* pipe1, char* pipe2){
	objet.manuel = manuel;
	objet.pipe1 = pipe1;
	objet.pipe2 = pipe2;
}

void captation_de_signal(pid_t pid){
	
	//processus père
	if (pid > 0){
		if (signal(SIGPIPE, mon_handler_pere) == SIG_ERR){
			unlink(objet.pipe1);
			unlink(objet.pipe2);
		}
		if (signal(SIGINT, mon_handler_pere) == SIG_ERR){
			unlink(objet.pipe1);
			unlink(objet.pipe2);
		}
	}

	
	//processus fils
	else if (pid == 0){
		if (signal(SIGPIPE, mon_handler_fils) == SIG_ERR){
			unlink(objet.pipe1);
			unlink(objet.pipe2);
		}
		if (signal(SIGINT, mon_handler_fils) == SIG_ERR){
			unlink(objet.pipe1);
			unlink(objet.pipe2);
		}
	}

}


void mon_handler_pere(int signal_recu){
	
	
	if (signal_recu == SIGPIPE){
		printf("Le pipe est fermé.\n");
		exit(11);
	}

	if (signal_recu == SIGINT && !*objet.manuel){
		printf("Vous venez d'appuyer sur CTRL+C et le pipe est fermé.\n");
		exit(12);
	}

	else if (signal_recu == SIGINT && *objet.manuel) {
		reset_mem_and_affiche("");
	}
}


void mon_handler_fils(int signal_recu){
	
	
	if (signal_recu == SIGINT && !*objet.manuel) {exit(12);}
	else if (signal_recu == SIGINT && *objet.manuel) {;}

	
	if (signal_recu == SIGPIPE) {exit(11);}
}

