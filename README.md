# SEGWAY_TR

Il s'agit d'un stage sur la réalisation et finalisation d'un maquette pédagoqique de type Segway. 

La gestion de supervision de temps réel est dans la partie de RPi3 et la commande de système est sur STM32.


Connexion:
----------
1.  La connexion entre 2 cartes est faite via l'interface UART.
2.  La connexion Bluetooth de Smartphone et Raspberry Pi 

Pour la simulation, la connexion est fait en serial (USB) de Raspberry Pi, MBED(ou STM 32) et PC 
	

Raspberry Pi 3:
----------
Description : 
La carte de Raspberry Pi 3 est la carte de superviseur du système de gyropode.
Le cadre applicatif (Framework) de temps réel qu'on a utilisé est Xenomai v3. Le noyau du système se fait en co-noyau de Linux et Cobalt de Xenomai. 


> **Fonctions:**
1. Supervision de système
2. Asservissement du sytème en temps réel
3. Gestion de cas urgent
4. Connectivité de bluetooth ( ajout de fonctionement future) 


STM32: 
----------
Description :
La carte STM32 Discovery est une carte de micro-controlleur qui a compris également un gyroscope et un accéleromètre. Cette micro-controlleur est servi pour la régulation de courant ( consigne par RPi 3) et renvoie les informations su sytème ( des données comme la vitesse, angle, direction de guidon, les courants de moteurs en temps réel à RPi 3 via UART)


> **Fonctions:**
1. Régulation de boucle de courant ( commande par Raspberry Pi 3 à 50 Hz)
2. Gestion de l'envoie des informations ( à 94 Hz) 



Partie Simulateur :
----------

Le simulateur est fait par la carte MBED (écrit en C++) et on a pu simuler le système de Segway à travers de la liaison série entre Raspberry Pi3 et MBED. On implémente les loi d'automatique de Segway pour faire les simulations de modèle mécanique et ses comportements de Segway.


L'interface graphique :
----------
> **1. Simulateur en PC:**
L'interface graphique (.Net) est écrit par Windows Forms en C sharp dans IDE Visual Studio. Il est executable en Windows 10 

> **2. GUI en RPi 3 :**
L'interface graphique est écrit en Python. Il affiche des informations en temps réel de STM 32 et les affichers. Il affiche également l'état de Segway 
