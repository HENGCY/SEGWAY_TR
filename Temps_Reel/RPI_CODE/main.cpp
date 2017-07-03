/* 
 * File:   main.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#include "includes.h"
#include <iostream>

void initStruct(void);
void startTasks(void);
void deleteTasks(void);

int main(void){

	/* disable memory swap */
	mlockall(MCL_CURRENT | MCL_FUTURE);	

	init_recording();
	initStruct();
	startTasks();
	pause();
	deleteTasks();

	return 0;
}

void initStruct(void) {
	int err;

	/* Creation des mutex */

	if(err = rt_mutex_create(&var_mutex_etat_com, "etat_com")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_arret, "arret")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_etat_angle, "etat_angle")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_consigne_courant, "consigne_courant")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_etat_calcul, "etat_calcul")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_status, "status")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_moteur, "moteur")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_batterie, "batterie")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_bat_warning, "bat_warning")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}


	/* Creation des taches */

	if (err = rt_task_create(&th_Communication, NULL, 0, PRIORITY_TH_COMMUNICATION, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Asservissement, NULL, 0, PRIORITY_TH_ASSERVISSEMENT, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Affichage, NULL, 0, PRIORITY_TH_AFFICHAGE, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Batterie, NULL, 0, PRIORITY_TH_BATTERIE, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Moteur, NULL, 0, PRIORITY_TH_MOTEUR, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_PresenceUser, NULL, 0, PRIORITY_TH_PRESENCEUSER, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
}

void startTasks() {
	int err;

	if (err = rt_task_start(&th_Communication, &Communication, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Asservissement, &Asservissement, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Affichage, &Affichage, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Batterie, &Batterie, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Moteur, &Moteur, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_PresenceUser, &PresenceUser, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

}

void deleteTasks() {

	rt_task_delete(&th_Communication);
	rt_task_delete(&th_Asservissement);
	rt_task_delete(&th_Affichage);
	rt_task_delete(&th_Batterie);
	rt_task_delete(&th_Moteur);
	rt_task_delete(&th_PresenceUser);
}


