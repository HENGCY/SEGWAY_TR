/* 
 * File:   fonctions.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */


#include "fonctions.h"
/*
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

		rt_printf("WAIT FOR SEMAPHORE!\n"); // Changer, mettre semaphore a la fin
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


	/* wait for task function termination 
	log_task_ended();
	stop_recording();
	rt_printf("END OF PROGRAM\n");
	
*/
	
}
/*
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
*/
 void Surveillance_Moteur(void *arg)
{
	log_task_entered();

	rt_printf("Thread surveillance_moteur : Debut de l'éxecution de periodique à 10 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 100000000);

	while (1) {
	
		rt_task_wait_period(NULL);
		
		rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);
	
		com = etat_com;
	
		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);
		
		if (com){
		
			rt_mutex_acquire(&var_mutex_moteur, TM_INFINITE);
			log_mutex_acquired(&var_mutex_moteur);
		
			/* Insérer contrôle de l'état des moteurs */
		
			rt_mutex_release(&var_mutex_moteur);
			log_mutex_released(&var_mutex_moteur);
		
	}
	log_task_ended();

}

 void Asservissement(void *arg)
{
	float angle, vit_angulaire, c1, c2, k1, k2; 
	log_task_entered();

	rt_printf("Thread Asservissement: Debut de l'exécution de periodique à 50 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 20000000);
	
	while (1) {
	
		rt_task_wait_period(NULL);
		
		rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);
	
		com = etat_com;
	
		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);
		
		if (com){
		
			rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
			log_mutex_acquired(&var_mutex_etat_angle);
			
			angle = angles_get_angle(etat_angle);
			vit_angulaire = angles_get_vitesse_ang(etat_angle);			
		
			rt_mutex_release(&var_mutex_etat_angle);
			log_mutex_released(&var_mutex_etat_angle);
			
			c1 = (k1 * angle + k2 * vit_angulaire)/0.10435;
			c2 = c1;
			
			rt_mutex_acquire(&var_mutex_consigne_courant, TM_INFINITE);
			log_mutex_acquired(&var_mutex_consigne_courant);
			
			consignes_set(consigne_courant,c1,c2);
		
			rt_mutex_release(&var_mutex_consigne_courant);
			log_mutex_released(&var_mutex_consigne_courant);
			
			rt_mutex_acquire(&var_mutex_etat_calcul, TM_INFINITE);
			log_mutex_acquired(&var_mutex_etat_calcul);
			
			etat_calcul = 1;
		
			rt_mutex_release(&var_mutex_etat_calcul);
			log_mutex_released(&var_mutex_etat_calcul);
			
		}

	}
	log_task_ended();

}

 void Presence_User(void *arg)
{
	int com, st;
	
	log_task_entered();

	rt_printf("Thread presence_user : Debut de l'éxecution de periodique à 10 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 100000000);
	
	while (1) {
	
		rt_task_wait_period(NULL);
		
		rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);
	
		com = etat_com;
	
		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);
		
		if (com){
		
			rt_mutex_acquire(&var_mutex_status, TM_INFINITE);
			log_mutex_acquired(&var_mutex_status);
		
			st = status;
		
			rt_mutex_release(&var_mutex_status);
			log_mutex_released(&var_mutex_status);
			
			if(st == NO_USER){
			
				rt_mutex_acquire(&var_mutex_arret, TM_INFINITE);
				log_mutex_acquired(&var_mutex_arret);
			
				arret = 1;
			
				rt_mutex_release(&var_mutex_arret);
				log_mutex_released(&var_mutex_arret);
				
			}
		}		
	}

	log_task_ended();

}
/*
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
*/
 void Surveillance_Batterie(void *arg)
{
	int com;
	Battery * bat = battery_new();
	
	log_task_entered();

	rt_printf("Thread Surveillance_Batterie : Debut de l'éxecution de periodique à 1 Hz\n");
	rt_task_set_periodic(NULL, TM_NOW, 1000000000);
	
	while (1) {
	
		rt_task_wait_period(NULL);
		
		rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);
	
		com = etat_com;
	
		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);
		
		if (com){
		
			rt_mutex_acquire(&var_mutex_batterie, TM_INFINITE);
			log_mutex_acquired(&var_mutex_batterie);
		
			battery_set_level(bat,battery_get_level(batterie));
		
			rt_mutex_release(&var_mutex_batterie);
			log_mutex_released(&var_mutex_batterie);
			
			if(bat->battery_get_level < 15){
				
				if(bat->battery_get_level < 5){
			
					rt_mutex_acquire(&var_mutex_arret, TM_INFINITE);
					log_mutex_acquired(&var_mutex_arret);
				
					arret = 1;
				
					rt_mutex_release(&var_mutex_arret);
					log_mutex_released(&var_mutex_arret);
					
				}
				else{
					
					rt_mutex_acquire(&var_mutex_bat_warning, TM_INFINITE);
					log_mutex_acquired(&var_mutex_bat_warning);
				
					bat_warning = 1;
				
					rt_mutex_release(&var_mutex_bat_warning);
					log_mutex_released(&var_mutex_bat_warning);
					
				}
			}			
		}		
	}
	
	log_task_ended();

}
/*
 void EcouteBluetooth(void *arg)
{
	log_task_entered();

	rt_printf("En ecoute du port Bluetooth\n");
		//while (1) {
			//Pour simuler l'�coute en permanance
			//rt_printf("Essai bluetooth\n"); 
		//}
	log_task_ended();

}*/


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


}