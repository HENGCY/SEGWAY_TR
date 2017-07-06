/* 
 * File:   fonctions.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */


#include "fonctions.h"

void Surveillance_Moteur(void *arg)
{
	log_task_entered();
        int com;

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
                       	
	}
	log_task_ended();

}

 void Asservissement(void *arg)
{
	float angle, vit_angulaire, c1, c2;
        int com;
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
			
			angle = etat_angle->get_angle(etat_angle);
			vit_angulaire = etat_angle->get_vitesse_ang(etat_angle);			
		
			rt_mutex_release(&var_mutex_etat_angle);
			log_mutex_released(&var_mutex_etat_angle);
			
			c1 = (k1 * angle + k2 * vit_angulaire)*PI/(180.0*0.80435);
			c2 = c1;
			
			rt_mutex_acquire(&var_mutex_consigne_courant, TM_INFINITE);
			log_mutex_acquired(&var_mutex_consigne_courant);
			
			consigne_courant->set(consigne_courant,c1,c2);
		
			rt_mutex_release(&var_mutex_consigne_courant);
			log_mutex_released(&var_mutex_consigne_courant);
			
			
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
		
			bat->set_level(bat, batterie->get_level(batterie));
		
			rt_mutex_release(&var_mutex_batterie);
			log_mutex_released(&var_mutex_batterie);
			
			if(bat->get_level(bat) < 15){
				
				if(bat->get_level(bat) < 5){
			
					rt_mutex_acquire(&var_mutex_arret, TM_INFINITE);
					log_mutex_acquired(&var_mutex_arret);
				
					arret = 1;
				
					rt_mutex_release(&var_mutex_arret);
					log_mutex_released(&var_mutex_arret);
					
				}
				else{
					rt_mutex_acquire(&var_mutex_batterie, TM_INFINITE);
                                        log_mutex_acquired(&var_mutex_batterie);
		
                                        batterie->set_warning(batterie,1);
		
                                        rt_mutex_release(&var_mutex_batterie);
                                        log_mutex_released(&var_mutex_batterie);

					
				}
			}			
		}
                
	}
	bat->free(bat);
	log_task_ended();

}


 void Communication(void *arg) 
{
	rt_printf("Started Serial\n");
	int uart0_filestream = -1;
        int com;
        int i;
	uart0_filestream = init_serial();

	if(uart0_filestream == -1){

		rt_printf("Can't Use the UART\n");
		rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);

		etat_com = 0;

		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);			
	}
	else{
		
		//----- TX BYTES -----
		unsigned char tx_buffer[20];
		unsigned char *p_tx_buffer;


		unsigned char message_buffer[256];
		int message_length = 0;
		float angular_position = 0;
		float angular_speed = 0;
		int arr;
		while (1) {
		
			rt_task_wait_period(NULL);
			
			rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
			log_mutex_acquired(&var_mutex_etat_com);

			com = etat_com;

			rt_mutex_release(&var_mutex_etat_com);
			log_mutex_released(&var_mutex_etat_com);			

			if (com){
			
				rt_mutex_acquire(&var_mutex_arret, TM_INFINITE);
				log_mutex_acquired(&var_mutex_arret);

				arr = arret;

				rt_mutex_release(&var_mutex_arret);
				log_mutex_released(&var_mutex_arret);
				
				if (arr = 1){
					send_float_to_serial(STOP, 't');
                                }
				
				message_serial *m;
                                m=read_from_serial();
;

				for(i=0;i<5;i++){
                                    
                                        if((m+i)->label =='p'){
						rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
						log_mutex_acquired(&var_mutex_etat_angle);

						etat_angle->set_angle(etat_angle,m[i].value);

						rt_mutex_release(&var_mutex_etat_angle);
						log_mutex_released(&var_mutex_etat_angle);	
					}
					else if((m+i)->label == 's'){
						rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
						log_mutex_acquired(&var_mutex_etat_angle);

                                                etat_angle->set_vitesse_ang(etat_angle,m[i].value); 

						rt_mutex_release(&var_mutex_etat_angle);
						log_mutex_released(&var_mutex_etat_angle);					
					}
					else if((m+i)->label == 'b'){
						rt_mutex_acquire(&var_mutex_batterie, TM_INFINITE);
						log_mutex_acquired(&var_mutex_batterie);

						batterie->set_level(batterie,m[i].value);

						rt_mutex_release(&var_mutex_batterie);
						log_mutex_released(&var_mutex_batterie);	
					}


					else if((m+i)->label == 'v'){
						rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
						log_mutex_acquired(&var_mutex_etat_angle);

                                                etat_angle->set_vitesse_lin(etat_angle,m[i].value); 

						rt_mutex_release(&var_mutex_etat_angle);
						log_mutex_released(&var_mutex_etat_angle);	
					}	
					else if((m+i)->label == 't'){
						rt_mutex_acquire(&var_mutex_status, TM_INFINITE);
						log_mutex_acquired(&var_mutex_status);

						status = (m+i)->value;

						rt_mutex_release(&var_mutex_status);
						log_mutex_released(&var_mutex_status);	
					}	
					else {
						rt_printf("Unknown message type : tag '%c'\n", m[i].label );
					}
				}	// end for message
			} // end if com
                        
		} // end while(1)

		//----- CLOSE THE UART -----
		close(uart0_filestream);
	}
    
}

void Affichage(void *arg){

	float angle, vit_ang, vit_lin, cons_cour1, cons_cour2;
	int bat_warning, com, bat_level;

	rt_task_wait_period(NULL);
			
	rt_mutex_acquire(&var_mutex_etat_com, TM_INFINITE);
	log_mutex_acquired(&var_mutex_etat_com);

	com = etat_com;

	rt_mutex_release(&var_mutex_etat_com);
	log_mutex_released(&var_mutex_etat_com);			

	if (com){

		rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_com);

		com = etat_com;

		rt_mutex_release(&var_mutex_etat_com);
		log_mutex_released(&var_mutex_etat_com);

                

		rt_mutex_acquire(&var_mutex_consigne_courant, TM_INFINITE);
		log_mutex_acquired(&var_mutex_consigne_courant);

		cons_cour1 = consigne_courant->get_moteur1(consigne_courant);
		cons_cour2 = consigne_courant->get_moteur2(consigne_courant);

		rt_mutex_release(&var_mutex_consigne_courant);
		log_mutex_released(&var_mutex_consigne_courant);

                

		rt_mutex_acquire(&var_mutex_etat_angle, TM_INFINITE);
		log_mutex_acquired(&var_mutex_etat_angle);

                angle=etat_angle->get_angle(etat_angle);
                vit_ang=etat_angle->get_vitesse_ang(etat_angle);
                vit_lin=etat_angle->get_vitesse_lin(etat_angle);
                
		rt_mutex_release(&var_mutex_etat_angle);
		log_mutex_released(&var_mutex_etat_angle);


                
		rt_mutex_acquire(&var_mutex_batterie, TM_INFINITE);
		log_mutex_acquired(&var_mutex_batterie);

                bat_level=batterie->get_level(batterie);
                bat_warning=batterie->get_warning(batterie);

		rt_mutex_release(&var_mutex_batterie);
		log_mutex_released(&var_mutex_batterie);


		// open program Python

    }
}
