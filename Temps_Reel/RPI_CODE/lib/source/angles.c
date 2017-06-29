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

#include "../headers/angles.h"
#include <stdio.h>

typedef struct Angles {
    void(*free) (struct Angles *This);                     /* Appel à la fonction angles_free(). */
    void(*set) (struct Angles *This, float theta, float vit ,float dir);/* Appel à la fonction angles_set().*/
    float(*get_angle) (struct Angles *This);               /* Appel à la fonction angles_get_angle(). */
    float(*get_vitesse_ang) (struct Angles *This);          /* Appel à la fonction angles_get_vitesse_ang().*/
    float(*get_direction) (struct Angles *This);           /* Appel à la fonction angles_get_direction().*/
    float angle;         /* Angle entre le guidon et la composante vertical du sol */
    float vitesse_ang;   /* Vitesse Angulaire entre le guidon et la composante vertical du sol*/
    float direction;     /* Angle entre la direction et mouvement de guidon.*/
} Angles;

static void angles_init(Angles*);

static void angles_init(Angles *This) {
    This->angle = ANGLE_INITIAL;
    This->vitesse_ang = VITESSE_ANG_INITIAL;
    This->direction= DIRECTION_INITIAL;
    This->get_angle = angles_get_angle;
    This->get_vitesse_ang = angles_get_vitesse_ang;
    This->get_direction= angles_get_direction;
    This->set =  angles_set;
}

Angles* angles_new(void) {
    Angles *this = malloc(sizeof(Angles));
    if (!This) return NULL;
    angles_init(This);
    This->free = angles_free;
    return This;
}

void angles_free(Angles  *This){
    free(This);
}

void angles_set(Angles *This, float theta, float vit ,float dir){
    This->angle = theta;
    This->vitesse_ang = vit;
    This->direction= dir;
}

float angles_get_angle(Angles *This){
    return This->angle;
}

float angles_get_vitesse_ang(Angles *This){
    return This->vitesse_ang;
}

float angles_get_direction(Angles *This);{
    return This->direction;
}
