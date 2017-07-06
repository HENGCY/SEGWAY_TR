/* 
 * File:   global.c
 * Author: INSA Toulouse
 *
 * Created on 20 May 2017
 */

#include "global.h"

/* Déclaration des variables partagées */

 int arret = 0;
 int etat_calcul = 0;
 int status = 0;
 int etat_com = 0;

 
 Angles*etat_angle;
 Battery* batterie;
 Consignes* consigne_courant;
 

 RT_SEM var_sem;

/* Déclaration des mutex */

 RT_MUTEX var_mutex_etat_com; 
 RT_MUTEX var_mutex_arret; 
 RT_MUTEX var_mutex_etat_angle;
 RT_MUTEX var_mutex_consigne_courant;
 RT_MUTEX var_mutex_etat_calcul;
 RT_MUTEX var_mutex_status;
 RT_MUTEX var_mutex_moteur;
 RT_MUTEX var_mutex_batterie;


/* @descripteurs des tâches */


RT_TASK th_Communication; 
RT_TASK th_Asservissement;
RT_TASK th_Affichage; 
RT_TASK th_Surveillance_Batterie; 
RT_TASK th_Surveillance_Moteur; 
RT_TASK th_Presence_User; 


