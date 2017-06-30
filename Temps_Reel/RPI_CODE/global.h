/* 
 * File:   global.h
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "includes.h"

#define TEN_SECONDS    (10*1e9)
#define SECENTOP       1200000000

#define MSG_LENGTH 50

extern int arret;
extern int etat_calcul;
extern int status;
extern int etat_com;

extern RT_SEM var_sem;

extern RT_MUTEX var_mutex_etat_com; 
extern RT_MUTEX var_mutex_arret; 
extern RT_MUTEX var_mutex_etat_angle;
extern RT_MUTEX var_mutex_consigne_courant;
extern RT_MUTEX var_mutex_etat_calcul;
extern RT_MUTEX var_mutex_status;
extern RT_MUTEX var_mutex_moteur;
extern RT_MUTEX var_mutex_batterie;
extern RT_MUTEX var_mutex_bat_warning;

/* @descripteurs des tâches */

extern RT_TASK Communication; 
extern RT_TASK Asservissement; 
extern RT_TASK Affichage; 
extern RT_TASK Batterie; 
extern RT_TASK Moteur; 
extern RT_TASK PresenceUser; 

#endif	/* GLOBAL_H */