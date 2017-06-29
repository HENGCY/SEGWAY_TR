/* 
 * File:   main.c
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */

#include "includes.h"
#include <iostream>

int main(void)
{

	/* disable memory swap */
	mlockall(MCL_CURRENT | MCL_FUTURE);	

	init_recording();

	rt_mutex_create(&var_mutex_etat_com, "etat_com");
	rt_mutex_create(&var_mutex_arret, "arret");
	rt_mutex_create(&var_mutex_etat_angle, "etat_angle");
	rt_mutex_create(&var_mutex_consigne_courant, "consigne_courant");
	rt_mutex_create(&var_mutex_etat_calcul, "etat_calcul");
	rt_mutex_create(&var_mutex_status, "status");
	rt_mutex_create(&var_mutex_moteur, "moteur");
	rt_mutex_create(&var_mutex_batterie, "batterie");
	rt_mutex_create(&var_mutex_vitesse, "vitesse");
	rt_mutex_create(&var_mutex_bat_warning, "bat_warning");

	rt_printf("Fin init mutex\n");
	rt_sem_create(&var_sem, "var_sem",0,TM_INFINITE);
	rt_printf("Fin init semaphore\n");

	
// Uncomment this if you want demo execution

//	if (rt_task_spawn(&mainTask, /* task descriptor */
//			"Main Task", /* name */
//			0           /* 0 = default stack size */,
//			0         /* priority */,
//			T_JOINABLE, /* needed to call rt_task_join after */
//			&MainTask,      /* entry point (function pointer) */
//			NULL        /* function argument */) != 0)
//		{
//			rt_printf("rt_task_spawn error\n");
//
//		}


//Uncomment this is you want serial execution
	if (rt_task_spawn(&serial, /* task descriptor */
			"Serial", /* name */
			0           /* 0 = default stack size */,
			0         /* priority */,
			T_JOINABLE, /* needed to call rt_task_join after */
			&Serial,      /* entry point (function pointer) */
			NULL        /* function argument */) != 0)
		{
			rt_printf("rt_task_spawn error\n");

		}

	pause();

	return 0;
}

