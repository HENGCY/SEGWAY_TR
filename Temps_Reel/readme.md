# Temps réel

Le programme de temps réel est exécuté sur la carte Raspberry Pi 3. Ce programme gère le monitoring du système, ainsi que l'asservissement en angle du Segway.
Nous avons distribué les fonctions en 6 threads :

### Communication
------------------------------------------------------

Le thread **Communication** se charge de récupérer périodiquement, à une fréquence de 100Hz, les informations reçues à l’aide de la liaison série du STM32. Il déchiffre les trames et mettre à jour les variables partagées utilisées par les autres threads.
Il doit d’abord vérifier que la communication est bien établie. Si ce n’est pas le cas, il met à false la variable *état_com*. Le thread se met ensuite en attente d’une trame sur la liaison série. A la réception de celle-ci, les données sont extraites et les variables partagées sont mises à jour en fonction, et la variable *état_com* est mise à l’état *true*.
Enfin, si la variable *etat_calcul* est à l’état *true*, le thread envoie au STM32 par la liaison série les consignes de courant qui ont été calculées par le thread **Asservissement**. Il change ensuite l’état de la variable *etat_calcul* à *false*.
Le thread est également chargé d’envoyer une trame d’arrêt du système si la variable *arrêt* est à l’état *false*.

### Asservissement
------------------------------------------------------

Le thread **Asservissement** effectue les calculs nécessaires au contrôle des déplacements du Segway. Si la variable *etat_com* est égale à *true*, il récupère les informations d’angle au niveau de la variable partagée *etat_angle*, et après son calcul, écrit les résultats dans la variable *consigne_courant* et met la variable *etat_calcul* à l’état *true*.
Ce thread fonctionne à une fréquence de 50 Hz.

### Surveillance moteur
------------------------------------------------------

Le thread **Surveillance Moteur** se charge de vérifier le bon fonctionnement des moteurs. Il consulte périodiquement, à une fréquence de 10 Hz, les informations contenues dans la variable partagée *moteur*.Si la variable *etat_com* est égale à *false*, la variable *moteur* n’est pas mise à jour. Si le moteur est en dysfonctionnement, la variable *arrêt* est mise à jour pour déclencher l’arrêt d’urgence.

### Surveillance Batterie
------------------------------------------------------

Le thread **Surveillance Batterie** se charge de vérifier le bon fonctionnement des moteurs. Il consulte périodiquement, à une fréquence de 1 Hz, les informations contenues dans la variable partagée *batterie*, tant que la variable *etat_com* est à l’état *true*. Si le niveau de batterie est faible, la variable *bat_warning* est mise à jour afin que l’utilisateur en soit informé par le thread **Affichage**. Enfin si le niveau de batterie atteint un niveau critique, la variable *arrêt* est mise à jour pour déclencher l’arrêt d’urgence.

### Affichage
------------------------------------------------------

Le thread **Affichage** se charge de communiquer à l’utilisateur toutes les informations importantes récoltées par le STM32. Pour cela il consulte certaines variables partagées et affiche leur état. Les valeurs sont rafraîchies à 2 Hz pour une bonne lisibilité. L’affichage n’est pas mis à jour si la variable *etat_com* est dans l’état *false*.

### Présence User
------------------------------------------------------

Le thread **Présence User** vérifie de manière périodique à 10 Hz la présence de l’utilisateur en consultant la variable partagée *présence_user*. Si cette variable est à l’état *false*, un compteur est incrémenté. Lorsque le compteur est égal à 5 (correspond à 500 ms d’absence d’appui sur le bouton), la variable *arrêt* est mise à l’état *true*, afin que l’arrêt d’urgence soit déclenché. De la même manière, le thread consulte la variable *etat_com*, et grâce à un autre compteur, il vérifie la durée du problème de communication et demande l’arrêt du système si ce compteur est égal à 2 (200 ms de perte de communication).
