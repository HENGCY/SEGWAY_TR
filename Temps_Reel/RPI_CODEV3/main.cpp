/* 
 * File:   main.c
 * Author: INSA Toulouse
 *
 * Created on 11-07
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

	if(err = rt_mutex_create(&var_mutex_etat_com, "mutex_etat_com")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_arret, "mutex_arret")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_etat_angle, "mutex_etat_angle")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_consigne_courant, "mutex_consigne_courant")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}


	if(err = rt_mutex_create(&var_mutex_status, "mutex_status")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_moteur, "mutex_moteur")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_batterie, "mutex_batterie")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if(err = rt_mutex_create(&var_mutex_etat_reception, "mutex_reception")){
		rt_printf("Error mutex create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
	/*if(err = rt_sem_create(&var_sem, "var_sem",0,TM_INFINITE)){
		rt_printf("Error semaphore create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}*/

        rt_printf("Fin init mutex\n");
	

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



/*
	if (err = rt_task_create(&th_Surveillance_Batterie, NULL, 0, PRIORITY_TH_BATTERIE, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Surveillance_Moteur, NULL, 0, PRIORITY_TH_MOTEUR, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_create(&th_Presence_User, NULL, 0, PRIORITY_TH_PRESENCEUSER, 0)){
		rt_printf("Error task create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
        */
		etat_angle=angles_new();
		batterie=battery_new();
		consigne_courant=consignes_new();
        rt_printf("Fin init tache\n");
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
/*
	if (err = rt_task_start(&th_Surveillance_Batterie, &Surveillance_Batterie, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Surveillance_Moteur, &Surveillance_Moteur, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}

	if (err = rt_task_start(&th_Presence_User, &Presence_User, NULL)){
		rt_printf("Error task start: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
*/
}

void deleteTasks() {

	rt_task_delete(&th_Communication);
	//log_task_deleted(&th_Communication);
	rt_task_delete(&th_Asservissement);
//	log_task_deleted(&th_Asservissement);
	rt_task_delete(&th_Affichage);
	//log_task_deleted(&th_Affichage);
/*
	rt_task_delete(&th_Surveillance_Batterie);
	rt_task_delete(&th_Surveillance_Moteur);
	rt_task_delete(&th_Presence_User);
        */
}


