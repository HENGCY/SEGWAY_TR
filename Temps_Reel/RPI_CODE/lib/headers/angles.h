/**
 * \file Angles.h
 * \brief Déclaration de la classe Angles.
 * \author INSA Toulouse
 * \version 1.0
 * \date 26 Juin 2017
 */

#ifndef ANGLES_H
#define ANGLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "./constantes.h"

    /**
     * \struct Angles
     * \brief Angles est un objet contenant la description de l'état d'angles de Segway.
     */
	 /*! Définition du type pour Angles */
    typedef struct Angles {
        void(*free) (struct Angles *This);                     /*!< Appel à la fonction angles_free(). */
        void(*set) (struct Angles *This, float theta, float vit ,float dir);/*!< Appel à la fonction angles_set().*/
        float(*get_angle) (struct Angles *This);               /*!< Appel à la fonction angles_get_angle(). */
        float(*get_vitesse_ang) (struct Angles *This);          /*!< Appel à la fonction angles_get_vitesse_ang().*/
        float(*get_direction) (struct Angles *This);           /*!< Appel à la fonction angles_get_direction().*/
        float angle;         /*!< Angle entre le guidon et composant vertical de Segway */
        float vitesse_ang;   /*!< Vitesse Angulaire entre le guidon et composant vertical de Segway*/
        float direction;     /*!< Angle entre la direction et mouvement de guidon.*/
    } Angles;

    /**
     * \fn Angles* angles_new(void)
     * \brief Instanciation de l'objet Angles.
     * \return Nouvelle instance de type Angles.
     */
    Angles* angles_new(void);

    /**
     * \fn void angles_free(Angles *This)
     * \brief Fonction pour désallouer un instance de l'objet Angles.
     * \param This Auto-adressage de l'objet à désallouer.
     */
    void angles_free(Angles *This);

    /**
     * \fn void angles_set (Angles *This, float theta, float vit ,float dir)
     * \brief Mise à jour des données décrit par un objet Angles.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \param theta Nouvelle valeur du angle (entre -20.0 20.0)
     * \param vit Nouvelle valeur du vitesse_ang
     * \param dir Nouvelle valeur du angle de direction ( entre -20.0 , 20.0)
	   */
    void angles_set(Angles *This, float theta, float vit ,float dir);

    /**
     * \fn float angles_get_angle(Angles *This)
     * \brief Lecture d'angle' d'un objet Angles.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
	   * \return Valeur angle. (entre -20.0 20.0)
     */
    float angles_get_angle(Angles *This);

    /**
     * \fn float angles_get_vitesse_ang(Angles *This)
     * \brief Lecture de vitesse_angulaire d'un objet Angles.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
	   * \return Valeur de vitesse_ang.
     */
    float angles_get_vitesse_ang(Angles *This);

    /**
     * \fn float angles_get_get_direction(Angles *This)
     * \brief Lecture de direction d'un objet Angles.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
	   * \return Valeur de angle de direction. (entre -20.0 20.0)
     */
    float angles_get_direction(Angles *This);


#ifdef __cplusplus
}
#endif
#endif
