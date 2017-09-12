#include "gui.h"
#include "global.h"
#include <stdio.h>

int sckt_log, sckt_gui;

int init_socket(int port){

	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr, serv_addr;

	/* command line: server [port_number] */
  
	//port = SERV_TCP_PORT;

	/* open a TCP socket (an Internet stream socket) */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("can't open stream socket");
		//exit(1);
	}

	/* bind the local address, so that the client can send to server */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("can't bind local address");
		//exit(1);
	}
	/* listen to the socket */
	listen(sockfd, 5);

	//while(1) {
	
     	/* wait for a connection from a client; this is an iterative server */
     	clilen = sizeof(cli_addr);
     	newsockfd = accept(sockfd, &cli_addr, &clilen);

     	if(newsockfd < 0) {
        	perror("can't bind local address");
     	}

	return newsockfd;
}

void read_socket_values(){
	int len,i,taille;
	unsigned char tab[MAX_SIZE];
	unsigned char string[MAX_SIZE];
	float data;

	/* read a message from the client */
 	len = read(sckt_gui, string, MAX_SIZE); 

 	/* make sure it's a proper string */
 	//string[len] = 0;
	taille=strlen((char*)string);
	if(taille > 0){
		memcpy(tab,string,taille);

		for(i=0 ; i<len ; i++){
				if((tab[i] == '<')&&(tab[i+6] == '\n')){
					switch (tab[i+1]){
						//couple
						case 'c':		
								memcpy(&data,&tab[i+2],sizeof(data));
								couple = data -100000.05f ; // retrait de l'offset ajouté par la RPI lors de l'envoi
								printf("%c: %f\n",tab[i+1],couple);
								break;	
						//angle beta
						case 'v' : 
								memcpy(&data,&tab[i+2],sizeof(data));
								vit_lin = (data -100000.05f)*180.0f/3.142f ; //conversion en degrés
								printf("%c: %f\n",tab[i+1],vit_lin);
								break;
						//battery
						case 'b' : 
								memcpy(&data,&tab[i+2],sizeof(data));							
								bat_lvl = data -100000.05f ;
								printf("%c: %d\n",tab[i+1],bat_lvl );
								break;
						//vitesse angulaire
						case 's' :
								memcpy(&data,&tab[i+2],sizeof(data));
								vit_ang = (data -100000.05f)*180.0f/3.142f; //conversion en degrés
								printf("%c: %f\n",tab[i+1],vit_ang);
								break;
						//angle
						case 'p' : 
								memcpy(&data,&tab[i+2],sizeof(data));
								angle = (data -100000.05f)*180.0f/3.142f; //conversion en degrés
								printf("%c: %f\n",tab[i+1],angle);
								break;
						//etat communication
						case 'e' : 
								memcpy(&data,&tab[i+2],sizeof(data));	
								etat_com = data -100000.05f  ;
								printf("%c: %d\n",tab[i+1],etat_com);
								break;	
						//presence user
						case 'u' : 
								memcpy(&data,&tab[i+2],sizeof(data));	
								presence =data -100000.05f ;
								printf("%c: %d\n",tab[i+1],presence);
								break;	
						//arret urgence
						case 'a' : 
								memcpy(&data,&tab[i+2],sizeof(data));	
								arret = data -100000.05f  ;
								printf("%c: %d\n",tab[i+1],arret);
								break;	
							
					}//case
				}//<\n
		} //for
	printf("\n");
	} //if taille

}

/* Actualise les infos communiquées a l'utilisateur */
gboolean update_info(GtkWidget *widget){

	char *str;
	int line = FALSE;
	str = malloc(256);	

	if (widget == NULL) return FALSE;
	
	if (bat_lvl < 7){
		str = strcpy(str,"ATTENTION !!!! Niveau de batterie critique !");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	else if (bat_lvl < 15){
		str = strcpy(str,"Batterie faible !");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	if (vit_lin > 4.0 && line == FALSE){
		str = strcpy(str,"Vitesse trop élevée !");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	else if (vit_lin > 4.0 && line == TRUE){
		str = strcat(str,"\nVitesse trop élevée");
		g_utf8_make_valid(str,strlen(str));
	}
	if (etat_com == 0 && line == FALSE){
		str = strcpy(str,"Problème de communication, arrêt du systeme");
		g_utf8_make_valid(str,strlen(str));
		line = TRUE;
	}
	else if (etat_com == 0 && line == TRUE){
		str = strcat(str,"\nProblème de communication, arrêt du système");
		g_utf8_make_valid(str,strlen(str));
	}
	if (line == FALSE){
		str = strcpy(str,"Rien à signaler");
		g_utf8_make_valid(str,strlen(str));
	}

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;

}

/* Fonction appelée periodiquement, qui actualise la valeur de l'angle */
gboolean update_angle(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",angle);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise la valeur de la vitesse angulaire*/
gboolean update_vit_ang(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",vit_ang);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise la valeur de la vitesse linéaire */
gboolean update_vit_lin(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",vit_lin);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise la valeur de la consigne de couple */
gboolean update_couple(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	snprintf(str,16,"%f",couple);

	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise la valeur du niveau de batterie*/
gboolean update_bat_lvl(GtkWidget *widget){
	char *str;
	char *str2;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	str2 = malloc(16);
	sprintf(str,"%d",bat_lvl);
	sprintf(str2," %c",'%');
	str = strcat(str,str2);

	/* Mise a jour des valeurs a afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise le contenu du log */
gboolean update_log(GtkWidget *widget){
	
	gchar lecture[MAX_SIZE];
	int len,taille;
	unsigned char string[MAX_SIZE];

	c++;

	if(c==2 && pass == 1){

		/* read a message from the client */
	 	len = read(sckt_log, string, MAX_SIZE); 

		if(len > 0){
			taille=strlen((char*)string);
			memcpy(lecture,string,taille);

			GtkTextIter iter_end;
			GtkTextIter iter_start;
			GtkTextBuffer *buf = gtk_text_buffer_new(NULL);

			if (widget == NULL) return FALSE;

			/* Recupération du buffer de l'espace de texte affichant le log */
			buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
			gtk_text_buffer_get_start_iter(buf,&iter_start);
			gtk_text_buffer_get_end_iter(buf,&iter_end);
			/* Possibilité grâce à la ligne suivante de supprimer le contenu de la fenêtre pour n'afficher que les dernière lignes de log reçues par le socket (retard par rapport au temps réel); */
			//gtk_text_buffer_delete(buf,&iter_start,&iter_end);

			/* Ecriture du log dans le fenetre dediée */
			g_print("AVANT INSERT : %s\n",lecture);
			gtk_text_buffer_insert(buf, &iter_end, g_utf8_make_valid (lecture,strlen(lecture)), -1);
			g_print("APRES INSERT\n");

			c=0;
		}
	}
	return TRUE;
}




/* Fonction appelée periodiquement, qui actualise la valeur de l'angle */
gboolean update_etat_communication(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	if (etat_com==1){
		snprintf(str,16,"Oui");
	}else if (etat_com==0){
		snprintf(str,16,"Non");
	}
	
	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise la valeur de l'angle */
gboolean update_arret(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	if (arret==1){
		snprintf(str,16,"Oui");
	}else if (arret==0){
		snprintf(str,16,"Non");
	}
	
	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}

/* Fonction appelée periodiquement, qui actualise la valeur de l'angle */
gboolean update_presence_user(GtkWidget *widget){
	char *str;

	if (widget == NULL) return FALSE;

	str = malloc(16);
	if (presence==1){
		snprintf(str,16,"Oui");
	}else if (presence==0){
		snprintf(str,16,"Non");
	}
	
	/* Mise a jour des valeurs à afficher */
	GtkTextBuffer *buf = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(buf,str,strlen(str));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget),buf);

	free(str);

	return TRUE;
}


/* If the provided string is valid UTF-8, return a copy of it.
 * If not, return a copy in which bytes that could not be interpreted
 * as valid Unicode are replaced with the Unicode replacement character (U+FFFD) */
gchar * g_utf8_make_valid (const gchar *str,gssize len)
{
  GString *string;
  const gchar *remainder, *invalid;
  gsize remaining_bytes, valid_bytes;

  g_return_val_if_fail (str != NULL, NULL);

  if (len < 0)
    len = strlen (str);

  string = NULL;
  remainder = str;
  remaining_bytes = len;

  while (remaining_bytes != 0) 
    {
      if (g_utf8_validate (remainder, remaining_bytes, &invalid)) 
	break;
      valid_bytes = invalid - remainder;
    
      if (string == NULL) 
	string = g_string_sized_new (remaining_bytes);

      g_string_append_len (string, remainder, valid_bytes);
      /* append U+FFFD REPLACEMENT CHARACTER */
      g_string_append (string, "\357\277\275");
      
      remaining_bytes -= valid_bytes + 1;
      remainder = invalid + 1;
    }
  
  if (string == NULL)
    return g_strndup (str, len);
  
  g_string_append (string, remainder);
  g_string_append_c (string, '\0');

  g_assert (g_utf8_validate (string->str, -1, NULL));

  return g_string_free (string, FALSE);
}
	
int pass=0;	

gboolean update_GUI (AppWidgets* app){
	gboolean test=FALSE;
	
	test=update_angle(app->valeur_angle);
	test=update_vit_lin(app->vitesse_lin);
	test=update_vit_ang(app->vitesse_angulaire);
	test=update_couple(app->couple);
	test=update_bat_lvl(app->bat_lvl);
	test=update_info(app->info);
	test=update_log(app->log);
	test=update_etat_communication(app->etat_communication);
	test=update_arret(app->arret);
	test=update_presence_user(app->presence_user);

	if(pass==0){
		printf("Attendre de la connexion de socket avec programme temps réel\n");
		sckt_log = init_socket(PORT_LOG);
		sckt_gui = init_socket(PORT_GUI);
		pass=1;
	}else{
		read_socket_values();
	}

	return test;
}

