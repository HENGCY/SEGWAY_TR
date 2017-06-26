/* 
 * File:   global.h
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "includes.h"

#define TEN_SECONDS             (10*1e9)
#define SECENTOP       1200000000

#define MSG_LENGTH 50

extern int mode; // 0 = arret total      1 = arret borne      2 = demarrage       3 = en fonctionnement        4 = arret HORS borne
extern int arret_def;

extern RT_SEM var_sem;

extern RT_MUTEX var_mutex_mode; 
extern RT_MUTEX var_mutex_arret_def ; 
extern RT_MUTEX var_mutex_var_cond;


/* @descripteurs des tâches */

extern RT_TASK mainTask;
extern RT_TASK batterie; 
extern RT_TASK ecouteBluetooth; 
extern RT_TASK checkBluetooth; 
extern RT_TASK moteur; 
extern RT_TASK asservissement; 
extern RT_TASK presence; 
extern RT_TASK rightUser; 
extern RT_TASK arretUrgence;
extern RT_TASK arretborne; 
extern RT_TASK arrethorsborne; 
extern RT_TASK nominal; 
extern RT_TASK demarrage;
extern RT_TASK serial;


#endif	/* GLOBAL_H */