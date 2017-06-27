/**
 * \file Consignes.h
 * \brief Déclaration de la classe Consignes.
 * \author INSA Toulouse
 * \version 1.0
 * \date 26 Juin 2017
 */

#ifndef CONSIGNES_H
#define CONSIGNES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "./constantes.h"

    /**
     * \struct Consignes
     * \brief Consignes est un objet contenant la description de commandes de couple en courant.
     */
	 /*! Définition du type pour Consignes */
    typedef struct Consignes {
        void(*free) (struct Consignes *This);                     /*!< Appel à la fonction Consignes_free(). */
        void(*set) (struct Consignes *This, float C1 , float C2); /*!< Appel à la fonction Consignes_set(). */
        float(*get_moteur1) (struct Consignes *This);             /*!< Appel à la fonction Consignes_get_moteur1(). */
        float(*get_moteur2) (struct Consignes *This);             /*!< Appel à la fonction Consignes_get_moteur2(). */
        float consigne_M1;   /*!< Consigne en ampere de Moteur 1. */
        float consigne_M2;   /*!< Consigne en ampere de Moteur 2. */
    } Consignes;

    /**
     * \fn Consignes* consignes_new(void)
     * \brief Instanciation de l'objet Consignes.
     * \return Nouvelle instance de type Consignes.
     */
    Consignes* consignes_new(void);

    /**
     * \fn void consignes_free(Consignes *This)
     * \brief Fonction pour désallouer un instance de l'objet Consignes.
     * \param This Auto-adressage de l'objet à désallouer.
     */
    void consignes_free(Consignes *This);

    /**
     * \fn void consignes_set(Consignes *This, float C1 , float C2)
     * \brief Mise à jour du comsignes de commandes de moteurs décrit par un objet Consignes.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \param C1  Nouvelle valeur de consigne M1
     * \param C2  Nouvelle valeur de consigne M2
	 */
    void consignes_set(Consignes *This, float C1 , float C2);

    /**
     * \fn float consignes_get_moteur1 (Consignes *This)
     * \brief Lecture du consigne_M1 d'un objet Consignes.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
  	 * \return float de consigne_M1
     */
    float consignes_get_moteur1(Consignes *This);

    /**
     * \fn float consignes_get_moteur2 (Consignes *This)
     * \brief Lecture du consigne_M2 d'un objet Consignes.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \return float de consigne_M2
     */
    float consignes_get_moteur2(Consignes *This);

#ifdef __cplusplus
}
#endif
#endif
