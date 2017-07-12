/* 
 * File:  fonctions.h
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#ifndef FONCTIONS_H
#define	FONCTIONS_H

#include "includes.h"
#include "lib/lib_monitor.h"

#ifdef	__cplusplus
extern "C" {
#endif

void Surveillance_Moteur(void * arg);
void Asservissement(void * arg);
void Presence_User(void * arg);
void Surveillance_Batterie(void * arg);
void Communication(void * arg);
void Affichage(void * arg);

#ifdef	__cplusplus
}
#endif

#endif	/* FONCTIONS_H */

