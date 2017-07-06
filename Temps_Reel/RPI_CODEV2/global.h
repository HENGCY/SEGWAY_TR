/* 
 * File:   global.h
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "includes.h"
#include "lib/lib.h"


#define TEN_SECONDS    (10*1e9)
#define SECENTOP       1200000000
#define MSG_LENGTH 50
 
/* Priorités des threads à définir */

#define PRIORITY_TH_COMMUNICATION  60
#define PRIORITY_TH_ASSERVISSEMENT  80
#define PRIORITY_TH_AFFICHAGE  70
#define PRIORITY_TH_BATTERIE  70
#define PRIORITY_TH_MOTEUR  70
#define PRIORITY_TH_PRESENCEUSER  70

#define STOP 99

#define k1  -25.8598f
#define k2  -6.5593f
#define PI  3.142f


extern int arret;
extern int status;
extern int etat_com;


extern Angles* etat_angle;
extern Battery* batterie;
extern Consignes* consigne_courant;

extern RT_SEM var_sem;

extern RT_MUTEX var_mutex_etat_com; 
extern RT_MUTEX var_mutex_arret; 
extern RT_MUTEX var_mutex_etat_angle;
extern RT_MUTEX var_mutex_consigne_courant;
extern RT_MUTEX var_mutex_status;
extern RT_MUTEX var_mutex_moteur;
extern RT_MUTEX var_mutex_batterie;


/* @descripteurs des tâches */

extern RT_TASK th_Communication; 
extern RT_TASK th_Asservissement;
extern RT_TASK th_Affichage; 
extern RT_TASK th_Surveillance_Batterie; 
extern RT_TASK th_Surveillance_Moteur; 
extern RT_TASK th_Presence_User; 



#endif	/* GLOBAL_H */
