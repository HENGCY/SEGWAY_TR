/* 
 * File:   fonctions.c
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */


#include "fonctions.h"

void MainTask(void *arg) {
	//add_task_to_monitoring(&moteur);
	int mode_save;

	log_task_entered();
	
	log_wait_for_mutex(&var_mutex_mode);
	rt_mutex_acquire(&var_mutex_mode,TM_INFINITE);
	log_mutex_acquired(&var_mutex_mode);
	
	mode = 1;
	mode_save = mode;
	rt_printf("Mode = 1\n");
	
	rt_mutex_release(&var_mutex_mode);
	log_mutex_released(&var_mutex_mode);
	
	open_mode(mode_save);

	int first_try = 0;
	
	log_wait_for_mutex(&var_mutex_arret_def);
	rt_mutex_acquire(&var_mutex_arret_def, TM_INFINITE);
	log_mutex_acquired(&var_mutex_arret_def);
	
	int arret_local = arret_def;
	rt_printf("Get arret def\n");
	
	rt_mutex_release(&var_mutex_arret_def);
	log_mutex_released(&var_mutex_arret_def);

	while (arret_local == 0) {

		if (first_try) {
			close_mode(mode_save);
			
			log_wait_for_mutex(&var_mutex_mode);
			rt_mutex_acquire(&var_mutex_mode, TM_INFINITE);
			log_mutex_acquired(&var_mutex_mode);
			
			mode_save = mode;
			rt_printf("Get save\n");
			
			rt_mutex_release(&var_mutex_mode);
			log_mutex_released(&var_mutex_mode);
			
			open_mode(mode_save);
		}
		else {
			first_try = 1;
		}

		rt_printf("WAIT FOR SEMAPHORE!\n"); // Changer , mettre semaphore a la fin
		log_sem_waiting(&var_sem);
		rt_sem_p(&var_sem, TM_INFINITE);
		log_sem_entered(&var_sem);

		log_wait_for_mutex(&var_mutex_arret_def);
		rt_mutex_acquire(&var_mutex_arret_def, TM_INFINITE);
		log_mutex_acquired(&var_mutex_arret_def);
		
		arret_local = arret_def;
		rt_printf("Get arret def\n");
		
		rt_mutex_release(&var_mutex_arret_def);
		log_mutex_released(&var_mutex_arret_def);
	}


	/* wait for task function termination */
	log_task_ended();
	stop_recording();
	rt_printf("END OF PROGRAM\n");
	

	
}

 void ArretUrgence(void *arg) {
	
	log_task_entered();
	
	rt_printf("Entree dans la procedure d'arret d'urgence\n");
	
	log_wait_for_mutex(&var_mutex_arret_def);
	rt_mutex_acquire(&var_mutex_arret_def, TM_INFINITE);
	log_mutex_acquired(&var_mutex_arret_def);
	
	arret_def = 1;
	rt_printf("Set arret def\n");
	
	rt_mutex_release(&var_mutex_arret_def);
	log_mutex_released(&var_mutex_arret_def);

	log_task_ended();
}

 void Moteur10(void *arg)
{
	log_task_entered();

	rt_task_set_periodic(NULL, TM_NOW, SECENTOP / 10);
	while (1) {
		rt_task_wait_period(NULL);
		rt_printf("Supervision moteur !\n");
	}

	log_task_ended();

}

 void Moteur(void *arg)
{
	log_task_entered();
	rt_task_set_periodic(NULL, TM_NOW, SECENTOP);
	while (1) {
		rt_task_wait_period(NULL);
		rt_printf("Supervision moteur !\n");
	}
	log_task_ended();

}

 void Asservissement(void *arg)
{
	log_task_entered();


	rt_task_set_periodic(NULL, TM_NOW, SECENTOP / 50);
	
	while (1) {
		rt_task_wait_period(NULL);
		rt_printf("Asservissement !\n");

	}
	log_task_ended();

}

 void Presence(void *arg)
{
	log_task_entered();


	rt_task_set_periodic(NULL, TM_NOW, SECENTOP / 10);
	
	while (1) {
		rt_task_wait_period(NULL);
		rt_printf("V�rification de la pr�sence de l'utilisateur!\n");
	}

	log_task_ended();

}

 void CheckUser(void *arg)
{
	log_task_entered();


	rt_task_set_periodic(NULL, TM_NOW, SECENTOP);
	while (1) {
		rt_task_wait_period(NULL);
		rt_printf("Check User!\n");
	}

	log_task_ended();

}

 void CheckBluetooth(void *arg)
{
	log_task_entered();

	rt_printf("V�rifie que le Bluetooth de l'utilisateur est a proximite.\n");
	//while (1) {
	//Pour simuler l'�coute en permanance
	//}
	log_task_ended();

}

 void Batterie(void *arg)
{
	log_task_entered();


	rt_task_set_periodic(NULL, TM_NOW, SECENTOP);
	while (1) {
		rt_task_wait_period(NULL);
		rt_printf("Supervision batterie !\n");
	}
	
	log_task_ended();

}

 void EcouteBluetooth(void *arg)
{
	log_task_entered();

	rt_printf("En ecoute du port Bluetooth\n");
		//while (1) {
			//Pour simuler l'�coute en permanance
			//rt_printf("Essai bluetooth\n"); 
		//}
	log_task_ended();

}

 void ArretBorne(void *arg)
{
	log_task_entered();

	rt_printf("Arret Borne !\n");

	if (rt_task_spawn(&batterie, /* task descriptor */
		"Batterie", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&Batterie,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement supervision batterie.\n");
	if (rt_task_spawn(&ecouteBluetooth, /* task descriptor */
		"Ecoute", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&EcouteBluetooth,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement Bluetooth.\n");
	rt_task_sleep(TEN_SECONDS);
	
	log_wait_for_mutex(&var_mutex_mode);
	rt_mutex_acquire(&var_mutex_mode, TM_INFINITE);
	log_mutex_acquired(&var_mutex_mode);

	mode = 2;
	rt_printf("Set mode 2\n");
	
	log_mutex_released(&var_mutex_mode);
	rt_mutex_release(&var_mutex_mode);
	
	log_sem_signaled(&var_sem);
	rt_sem_v(&var_sem);

	log_task_ended();


}

 void Demarrage(void *arg)
{
	log_task_entered();


	rt_printf("Demarrage !\n");
	if (rt_task_spawn(&batterie, /* task descriptor */
		"Batterie", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&Batterie,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}
	rt_printf("Lancement supervision batterie.\n");

	if (rt_task_spawn(&moteur, /* task descriptor */
		"Moteur", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&Moteur,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement supervision moteur.\n");
	if (rt_task_spawn(&checkBluetooth, /* task descriptor */
		"Check Bluetooth", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&CheckBluetooth,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_task_sleep(TEN_SECONDS);
	
	log_wait_for_mutex(&var_mutex_mode);
	rt_mutex_acquire(&var_mutex_mode, TM_INFINITE);
	log_mutex_acquired(&var_mutex_mode);
	
	mode = 3;
	rt_printf("Set mode 3\n");
	
	rt_mutex_release(&var_mutex_mode);
	log_mutex_released(&var_mutex_mode);
	
	log_sem_signaled(&var_sem);
	rt_sem_v(&var_sem);

	log_task_ended();
}

 void Nominal(void *arg)
{
	log_task_entered();


	rt_printf("Nominal GO!\n");
	if (rt_task_spawn(&asservissement, /* task descriptor */
		"Asservissement", /* name */
		0           /* 0 = default stack size */,
		60          /* priority */,
		0, /* needed to call rt_task_join after */
		&Asservissement,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}
	rt_printf("Lancement asservissement.\n");

	if (rt_task_spawn(&batterie, /* task descriptor */
		"Batterie", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&Batterie,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement supervision batterie.\n");

	if (rt_task_spawn(&moteur, /* task descriptor */
		"Moteur", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&Moteur10,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement supervision moteur.\n");

	if (rt_task_spawn(&presence, /* task descriptor */
		"Presence", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&Presence,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement supervision utilisateur.\n");

	if (rt_task_spawn(&rightUser, /* task descriptor */
		"Checking", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&CheckUser,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}
	if (rt_task_spawn(&ecouteBluetooth, /* task descriptor */
		"Ecoute", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&EcouteBluetooth,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Mode nominal lance.\n");
	rt_task_sleep(TEN_SECONDS);
	
	log_wait_for_mutex(&var_mutex_mode);
	rt_mutex_acquire(&var_mutex_mode, TM_INFINITE);
	log_mutex_acquired(&var_mutex_mode);
	
	mode = 4;
	rt_printf("Set mode 4\n");
	
	rt_mutex_release(&var_mutex_mode);
	log_mutex_released(&var_mutex_mode);
	
	log_sem_signaled(&var_sem);
	rt_sem_v(&var_sem);

	log_task_ended();

}


 void ArretHorsBorne(void *arg)
{
	log_task_entered();

	rt_printf("Arret Hors Borne !\n");

	if (rt_task_spawn(&ecouteBluetooth, /* task descriptor */
		"Ecoute", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&EcouteBluetooth,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}

	rt_printf("Lancement Bluetooth.\n");

	rt_task_sleep(TEN_SECONDS);

	rt_printf("Declenchement de la procedure d'arret d'urgence\n");
	if (rt_task_spawn(&arretUrgence, /* task descriptor */
		"Urgence", /* name */
		0           /* 0 = default stack size */,
		30          /* priority */,
		0, /* needed to call rt_task_join after */
		&ArretUrgence,      /* entry point (function pointer) */
		NULL        /* function argument */) != 0)
	{
		rt_printf("rt_task_spawn error\n");
	}
	
	log_sem_signaled(&var_sem);
	rt_sem_v(&var_sem);
	
	rt_printf("FIN DE PROGRAMME\n");

	log_task_ended();

}


 void Serial(void *arg) 
{
		rt_printf("Started Serial\n");
		int uart0_filestream = -1;

		uart0_filestream = init_serial();

		if(uart0_filestream == -1){
			rt_printf("Can't Use the UART\n");	
		}else{

			
			//----- TX BYTES -----
			unsigned char tx_buffer[20];
			unsigned char *p_tx_buffer;

	
			unsigned char message_buffer[256];
			int message_length = 0;
			float angular_position = 0;
			float angular_speed = 0;

	
			while (1) {
				message_serial m = read_from_serial();
				if(m.label =='p'){
					angular_position = m.value;
				}
				else if(m.label == 's'){
					angular_speed = m.value;

					float torque = -1*(angular_position *(-25.6343f) + angular_speed*(-6.4466f)) ;
					message_length = 0;
					
					send_float_to_serial(torque, 't');

					rt_printf("Angular position :  %f\n", angular_position);
					rt_printf("Angular speed : %f\n", angular_speed);
					rt_printf("torque = %f\n", torque);
				}
				else {
					rt_printf("Unknown message type : tag '%c'\n", m.label );
				}	
			}
			

			//----- CLOSE THE UART -----
			close(uart0_filestream);
		}
}



void close_mode(int mode) 
{
	if (mode == 1) {
		
		log_task_deleted(&arretborne);
		rt_task_delete(&arretborne);
		rt_printf("End Mode 1\n");
		
		log_task_deleted(&batterie);
		rt_task_delete(&batterie);
		rt_printf("Task Batterie deleted\n");
		
		log_task_deleted(&ecouteBluetooth);
		rt_task_delete(&ecouteBluetooth);
		rt_printf("Task Bluetooth deleted\n");
	}
	else if (mode == 2) {
		log_task_deleted(&demarrage);
		rt_task_delete(&demarrage);
		rt_printf("End Mode 2\n");

		log_task_deleted(&batterie);
		rt_task_delete(&batterie);
		rt_printf("Task Batterie deleted\n");

		log_task_deleted(&moteur);
		rt_task_delete(&moteur);
		rt_printf("Task Moteur deleted\n");
	}
	else if (mode == 3) {
		log_task_deleted(&nominal);
		rt_task_delete(&nominal);
		rt_printf("End Mode 3\n");

		log_task_deleted(&batterie);
		rt_task_delete(&batterie);
		rt_printf("Task Batterie deleted\n");

		log_task_deleted(&moteur);
		rt_task_delete(&moteur);
		rt_printf("Task Moteur deleted\n");
		
		log_task_deleted(&asservissement);
		rt_task_delete(&asservissement);
		rt_printf("Task Asservissement deleted\n");

		log_task_deleted(&presence);
		rt_task_delete(&presence);
		rt_printf("Task Presence deleted\n");

		log_task_deleted(&rightUser);
		rt_task_delete(&rightUser);
		rt_printf("Task Verification User deleted\n");

		log_task_deleted(&ecouteBluetooth);
		rt_task_delete(&ecouteBluetooth);
		rt_printf("Task Ecoute Bluetooth deleted\n");
		
	}
	else if (mode == 4) {
		log_task_deleted(&arrethorsborne);
		rt_task_delete(&arrethorsborne);
		rt_printf("End Mode 4\n"); 

		log_task_deleted(&ecouteBluetooth);
		rt_task_delete(&ecouteBluetooth);
		rt_printf("Task Ecoute Bluetooth deleted\n");
	}
}



void open_mode(int mode) {

	if (mode == 1) {
		if (rt_task_spawn(&arretborne, /* task descriptor */
			"Arret Borne", /* name */
			0           /* 0 = default stack size */,
			40          /* priority */,
			0, /* needed to call rt_task_join after */
			&ArretBorne,      /* entry point (function pointer) */
			NULL        /* function argument */) != 0)
		{
			rt_printf("rt_task_spawn error\n");

		}
	}
	else if (mode == 2) {
		if (rt_task_spawn(&demarrage, /* task descriptor */
			"Demarrage", /* name */
			0           /* 0 = default stack size */,
			90          /* priority */,
			T_JOINABLE, /* needed to call rt_task_join after */
			&Demarrage,      /* entry point (function pointer) */
			NULL        /* function argument */) != 0)
		{
			rt_printf("rt_task_spawn error\n");

		}
	}
	else if (mode == 3) {
		if (rt_task_spawn(&nominal, /* task descriptor */
			"Nominal", /* name */
			0           /* 0 = default stack size */,
			90          /* priority */,
			T_JOINABLE, /* needed to call rt_task_join after */
			&Nominal,      /* entry point (function pointer) */
			NULL        /* function argument */) != 0)
		{
			rt_printf("rt_task_spawn error\n");

		}
	}
	else if (mode == 4) {
		if (rt_task_spawn(&arrethorsborne, /* task descriptor */
			"Arret Hors Borne", /* name */
			0           /* 0 = default stack size */,
			90          /* priority */,
			T_JOINABLE, /* needed to call rt_task_join after */
			&ArretHorsBorne,      /* entry point (function pointer) */
			NULL        /* function argument */) != 0)
		{
			rt_printf("rt_task_spawn error\n");

		}
	}

}