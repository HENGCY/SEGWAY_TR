# Matlab
Il s'agit un dossier Matlab qui contenant des codes pour simuler et trouver les lois d'automatiques pour la commande de Segway. 

La modélisation d'asservissement du système complète est donné par le professeur INSA Toulouse, M. Ion Hazyuk. 
Il s'agit la commande par correcteur retour d'état.

--------------------------------------------------------------------------------------------------------------

### Système Complète

Dans notre système , le consigne pour theta (angle entre vertical du système au sol et le guidon) est zéro car nous décidons de bien stabiliser le système et mettre toujours le Segway en position vertical.

Le système est donc commandé par Beta ( angle entre l'utilisateur et le guidon), qui est considéré comme une pertubation de système.
L'asservissement du système se fait dans la carte Raspberry Pi et il envoie tous les 50 Hz à la carte STM32 pour la commande en couple.

                     | Beta
                     |
    Couple   ________|___________________       theta (angle)
       -----|  Système mécanique         |----------------------------------
       |    |  du segway                 |                                  |
       |    |                            |    d_theta(vitesse angulaire)    |
       |    |____________________________|-------------------------------   |
       |                                                                 |  |
       |           _____      ____                                       |  |
       |----------|  +  |----| k2 |--------------------------------------|  |
                  |     |    |____|                                         |
                  |  +  |     ____                                          |
                  |_____|----| k1 |-----------------------------------------
                             |____|
                             
----------------------------------------------------------------------------------------------------------------------------------

### Système boucle de courant

Dans notre système , le consigne de couple est reçu tous les 50 Hz pour STM 32 et il converti d'abord les consignes de courant correspondant pour les couples de retour états.

Le système est donc commandé par Beta ( angle entre l'utilisateur et le guidon), qui est considéré comme une pertubation de système.
L'asservissement du système se fait dans la carte Raspberry Pi et il envoie tous les 50 Hz à la carte STM32 pour la commande en couple.
Le moteur est commandé en tension par PWM pour atteindre le couple souhaité par le système et on utilise le correcteur de PI pour la régulation de courant 

                             
         (en A)         err(en A)    ________    Commande (en V)     ______
     Consigne_I-->+( )--------------| Cor_PI |----------->----------|Moteur|--->------- I
                   -^               |________|                      |______|    |
                    |                                                           |
                    |-----------------------------------------------------------|
							 		
