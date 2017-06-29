/**
 * \file Consignes.c
 * \brief Source de la classe Consignes
 * \author INSA Toulouse
 * \version 1.0
 * \date 27 Juin 2017
 *
 * Implémentation de la classe Consignes
 *
 */

#include "../headers/consignes.h"
#include <stdio.h>

static void consignes_init(Consignes*);

static void consignes_init(Consignes *This) {
    This->consigne_M1 = CONSIGNE_INITIAL;
    This->consigne_M2 = CONSIGNE_INITIAL;
    This->get_moteur1 = consignes_get_moteur1;
    This->get_moteur2 = consignes_get_moteur2;
    This->set = consignes_set;
}

Consignes* consignes_new(void) {
    Consignes *this = malloc(sizeof(Consignes));
    if (!This) return NULL;
    consignes_init(This);
    This->free = consignes_free;
    return This;
}

void consignes_free(Consignes *This){
    free(This);
}

void consignes_set(Consignes *This, float C1 , float C2){
    This->consigne_M1 = C1;
    This->consigne_M2 = C2;
}

float consignes_get_moteur1(Consignes *This){
    return This->consigne_M1;
}

float consignes_get_moteur2(Consignes *This){
    return This->consigne_M2;
}
