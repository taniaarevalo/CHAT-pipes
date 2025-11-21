#ifndef GESTION_SIGNAL_H
#define GESTION_SIGNAL_H

#include <stdbool.h> //bool
#include <sys/types.h> //pid_t

typedef struct Ce_qu_il_faut {
	
	bool* manuel;
	char* pipe1;
	char* pipe2;
	
}Cqif;

void init_sig_obj(bool* manuel, char* pipe1, char* pipe2);
void captation_de_signal(pid_t pid);
void mon_handler_pere(int signal_recu);
void mon_handler_fils(int signal_recu);


#endif
