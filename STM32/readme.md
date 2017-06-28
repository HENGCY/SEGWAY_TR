# STM32
Il s'agit de codes implémenté dans la carte de STM32F303 Discovery. 
Les codes sont écrits en C dans IDE de Keil.

Les codes sont contenant des fonctions des initialisations de peripheriques ( Timer, PWM, USART- DMA, ADC- DMA, Interruption EXTI).
Les codes sont dans le dossier Projet, et le dossier de documents contient des documents de modelisation de la carte electronique et les dessine de routage. Les datasheet de microcotrolleurs, accelerometre et gyroscope sont aussi dedans.

Le dossier library est necessaire pour la carte STM 32. Il s'agit un libraire bien fait par ST et on a utilisé directement 

La régulation de boucle de courant est fait aussi. 

Choses A faire :
1. la gestion de communication entre Raspberry Pi
2. la gestion de protection d'utilisateur
3. la gestion de la direction de guidon 
