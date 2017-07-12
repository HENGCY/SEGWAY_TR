/**
 * \file angles.c
 * \brief Source de la classe Angles
 * \author INSA Toulouse
 * \version 1.0
 * \date 27 Juin 2017
 *
 * Implémentation de la classe Angles
 *
 */

#include "angles.h"
#include <stdio.h>


static void angles_init(Angles*);

static void angles_init(Angles *This) {
    This->angle = ANGLE_INITIAL;
    This->vitesse_ang = VITESSE_ANG_INITIAL;
    This->direction= DIRECTION_INITIAL;
    This->vitesse_lin= VITESSE_LIN_INITIAL;

    This->get_angle = angles_get_angle;
    This->get_vitesse_ang = angles_get_vitesse_ang;
    This->get_direction= angles_get_direction;
    This->get_vitesse_lin= angles_get_vitesse_lin;

    This->set =  angles_set;

    This->set_angle =  angles_set_angle;
    This->set_vitesse_ang =  angles_set_vitesse_ang;
    This->set_direction =  angles_set_direction;
    This->set_vitesse_lin =  angles_set_vitesse_lin;
}

Angles* angles_new(void) {
    Angles *This = (Angles *)malloc(sizeof(Angles));
    if (!This) return NULL;
    angles_init(This);
    This->free = angles_free;
    return This;
}

void angles_free(Angles  *This){
    free(This);
}

void angles_set(Angles *This, float theta, float thetap ,float dir,float vit){
    This->angle = theta;
    This->vitesse_ang = thetap;
    This->direction= dir;
    This->vitesse_lin = vit;
}

void angles_set_angle(Angles *This, float theta){
    This->angle = theta;
}

void angles_set_vitesse_ang(Angles *This,float thetap){
    This->vitesse_ang = thetap;
}

void angles_set_direction(Angles *This,float dir){
    This->direction= dir;
}

void angles_set_vitesse_lin(Angles *This,float vit){
    This->vitesse_lin = vit;
}

float angles_get_angle(Angles *This){
    return This->angle;
}

float angles_get_vitesse_ang(Angles *This){
    return This->vitesse_ang;
}

float angles_get_direction(Angles *This){
    return This->direction;
}

 float angles_get_vitesse_lin(Angles *This){
    return This->vitesse_lin;
}

 
