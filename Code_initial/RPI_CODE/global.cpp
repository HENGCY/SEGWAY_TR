/* 
 * File:   global.c
 * Author: Romain Rivière, Alexandre Benazech, Vincent Pera
 *
 * Created on 20 May 2017
 */


#include "global.h"



 int mode = 0; // 0 = arret total      1 = arret borne      2 = demarrage       3 = en fonctionnement        4 = arret HORS borne
 int arret_def = 0;

 RT_SEM var_sem;

 RT_MUTEX var_mutex_mode; 
 RT_MUTEX var_mutex_arret_def ; 
 RT_MUTEX var_mutex_var_cond;


/* @descripteurs des tâches */

RT_TASK batterie; 
RT_TASK ecouteBluetooth; 
RT_TASK checkBluetooth; 
RT_TASK moteur; 
RT_TASK asservissement; 
RT_TASK presence; 
RT_TASK rightUser; 
RT_TASK arretUrgence;
RT_TASK arretborne; 
RT_TASK arrethorsborne; 
RT_TASK nominal; 
RT_TASK demarrage;
RT_TASK mainTask;
RT_TASK serial;


