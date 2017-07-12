/**
 * \file battery.h
 * \brief Déclaration de la classe Battery.
 * \author INSA Toulouse
 * \version 1.0
 * \date 26 Juin 2017
 */

#ifndef BATTERY_H
#define BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "constantes.h"

    /**
     * \struct Battery
     * \brief Battery est un objet contenant la description de l'état de la batterie.
     */
	 /*! Définition du type pour Battery */
    typedef struct Battery {
        void(*free) (struct Battery *This);                         /*!< Appel à la fonction battery_free(). */
        void(*set_level) (struct Battery *This, int level);         /*!< Appel à la fonction battery_set_level(). */
        void(*set_warning) (struct Battery *This, int bat_warning); /*!< Appel à la fonction battery_set_warning(). */
        int(*get_level) (struct Battery *This);                     /*!< Appel à la fonction battery_get_level(). */
        int(*get_warning) (struct Battery *This);                   /*!< Appel à la fonction battery_get_warning(). */
        int level;              /*!< Niveau de la batterie. */
        int bat_warning;        /*!< Indication de etat de la batterie. */ 
    } Battery;

    /**
     * \fn Battery* battery_new(void)
     * \brief Instanciation de l'objet Battery.
     * \return Nouvelle instance de type Battery.
     */
    Battery* battery_new(void);

    /**
     * \fn void battery_free(Battery *This)
     * \brief Fonction pour désallouer un instance de l'objet Battery.
     * \param This Auto-adressage de l'objet à désallouer.
     */
    void battery_free(Battery *This);

    /**
     * \fn void battery_set_level(Battery *This, int level)
     * \brief Mise à jour du niveau de la batterie décrit par un objet Battery.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \param level Nouvelle valeur du niveau de la batterie. Les valeurs possibles
	   * sont entre valeurs entiers (0 - 100) et #BATTERY_LEVEL_UNKNOWN (voir constantes.h)
	   */
    void battery_set_level(Battery *This, int level);

    /**
     * \fn void battery_set_warning(Battery *This, int bat_warning)
     * \brief Mise à jour du niveau de la batterie décrit par un objet Battery.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \param level Nouvelle etat du niveau de la batterie. Les valeurs possibles
    * sont entre valeurs entiers et #BATTERY_WARNING_UNKNOWN (voir constantes.h)
     */
    void battery_set_warning(Battery *This, int bat_warning);
    

    /**
     * \fn int battery_get_level(Battery *This)
     * \brief Lecture du niveau d'un objet Battery.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \return Niveau de la batterie.  Les valeurs possibles sont entre valeurs entiers
     * (0 - 100) et #BATTERY_LEVEL_UNKNOWN (voir constantes.h)
     */
    int battery_get_level(Battery *This);

    /**
     * \fn int battery_get_warning(Battery *This)
     * \brief Lecture du niveau d'un objet Battery.
     * \param This Auto-adressage de l'objet auquel changer la valeur.
     * \return etat warning de la batterie.  Les valeurs possibles sont entre valeurs entiers
     * et #BATTERY_WARNING_UNKNOWN (voir constantes.h)
     */
    int battery_get_warning(Battery *This);
    

#ifdef __cplusplus
}
#endif
#endif
