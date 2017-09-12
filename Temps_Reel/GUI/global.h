#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define PORT_GUI 8000
#define PORT_LOG 8001
#define MAX_SIZE 1024 /*Taille maximum en octets du string lu sur le socket_log, contenant des lignes de log */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/mman.h>
#include <time.h>
#include "gui.h"
#include <signal.h>
#include <malloc.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/file.h>

/* Declaration des variables partagees*/
 double angle, vit_ang, vit_lin, couple;
 int bat_lvl, etat_com, sckt_gui, sckt_log;
 int pass,arret,presence,c;

/* Structure contenant les differents Widgets a afficher */
typedef struct {
	GtkWidget *mywindow;
	GtkWidget *valeur_angle;
	GtkWidget *vitesse_angulaire;
	GtkWidget *vitesse_lin;
	GtkWidget *couple;
	GtkWidget *bat_lvl;
	GtkWidget *etat_communication;
	GtkWidget *arret;
	GtkWidget *presence_user;
	GtkWidget *scroll;
	GtkWidget *log;
	GtkWidget *info;
	GtkWidget *top;
} AppWidgets;

#endif
