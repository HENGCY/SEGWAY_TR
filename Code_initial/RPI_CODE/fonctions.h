/* 
 * File:  fonctions.h
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */

#ifndef FONCTIONS_H
#define	FONCTIONS_H

#include "includes.h"

#ifdef	__cplusplus
extern "C" {
#endif
void MainTask(void *arg); 
void ArretUrgence(void *arg);
void Moteur10(void *arg);
void Moteur(void *arg);
void Asservissement(void *arg);
void Presence(void *arg);
void CheckUser(void *arg);
void CheckBluetooth(void *arg);
void Batterie(void *arg);
void EcouteBluetooth(void *arg);
void ArretBorne(void *arg);
void Demarrage(void *arg);
void Nominal(void *arg);
void ArretHorsBorne(void *arg);
void Serial(void *arg);


void open_mode(int mode);
void close_mode(int mode);



#ifdef	__cplusplus
}
#endif

#endif	/* FONCTIONS_H */

