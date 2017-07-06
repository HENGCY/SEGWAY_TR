/**
 * \file battery.c
 * \brief Source de la classe Battery
 * \author INSA Toulouse
 * \version 1.0
 * \date 27 Juin 2017
 *
 * Implémentation de la classe Battery
 *
 */

#include "battery.h"
#include <stdio.h>

static void battery_init(Battery*);

static void battery_init(Battery *This) {
    This->level = BATTERY_LEVEL_UNKNOWN;
    This->bat_warning = BATTERY_WARNING_UNKNOWN;
    This->get_level = battery_get_level;
    This->set_level = battery_set_level;
    This->get_warning = battery_get_warning;
    This->set_warning = battery_set_warning;
}

Battery* battery_new(void) {
    Battery *This = (Battery*)malloc(sizeof(Battery));
    if (!This) return NULL;
    battery_init(This);
    This->free = battery_free;
    return This;
}

void battery_free(Battery *This){
    free(This);
}

void battery_set_level(Battery *This, int level){
    This->level = level;
}

int battery_get_level(Battery *This){
    return This->level;
}

void battery_set_warning(Battery *This, int bat_warning){
    This->bat_warning = bat_warning;
}

int battery_get_warning(Battery *This){
    return This->bat_warning;
}
