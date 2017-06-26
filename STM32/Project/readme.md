# Programmation de STM32
----------
Description :

La carte STM32 Discovery est une carte de micro-controlleur qui a compris également un gyroscope et un accéleromètre. 
Cette micro-controlleur est servi pour la régulation de courant ( consigne par RPi 3) et renvoie les informations du sytème ( des données comme la vitesse, angle, direction de guidon, les courants de moteurs en temps réel à RPi 3 via UART)



Utilisation:
----------
On a fait 3 types de correcteur de courant pour la regulation (boucle) de courant.
1. Regulateur Simple Gain proportional ( il existe de erreur statique de 37% )
2. Regulateur de type PI 
3. Regulateur de type PI anti wind up ( pour limiter la partie de correcteur integral)

Pour l'utiliser, il suffit de changer (ou remplacer) les codes de fonction.h et fonction.c dans le projet de Correcteur Kp propre.
	
