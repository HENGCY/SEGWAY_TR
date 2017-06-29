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
 RT_MUTEX var_mutex_vitesse;
 RT_MUTEX var_mutex_bat_warning;

/* @descripteurs des tâches */

RT_TASK Communication; 
RT_TASK Asservissement; 
RT_TASK Affichage; 
RT_TASK Batterie; 
RT_TASK Moteur; 
RT_TASK PresenceUser; 

