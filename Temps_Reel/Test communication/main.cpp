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

	init_serial();
	message_serial * mess[5];
	
	while(1){
		mess = read_from_serial();
		printf_trame(&mess);
	}

	return 0;
}

void printf_trame(message_serial *m){

	for (i=0;i<6;i++){
		switch (m[i]->label){
		case 'p':
			printf("Position angulaire: %d\n",m[i]->value);
			break;
		case 's':
			printf("Vitesse angulaire: %d\n",m[i]->value);
			break;
		case 'b':
			printf("Niveau de batterie: %d\n",m[i]->value);
			break;
		case 'v':
			printf("Vitesse linéaire: %d\n",m[i]->value);
			break;
		case 's':
			printf("Status: %d\n",m[i]->value);
			break;
		}
	}
}
