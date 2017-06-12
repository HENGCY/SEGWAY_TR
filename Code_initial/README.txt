Cette archive contient le code du projet tutoré Segway de 4IR-I mené en 2017 par Romain Rivière, Alexandre Benazech et Vincent Pere.
Ce projet a été réalisé sous la tutelle de Claude Baron et de Ion Haziuk.

Le dossier GUI contient le code source de l'interface graphique de suivi du simulateur
Il a été développé en C# (Winform .NET) et doit être ouvert avec Visual Studio. 
L'exécutable SegwayMonitor.exe se situe dans \GUI\SegwayMonitor\SegwayMonitor\bin\Debug.

Il a été testé sur un PC Windows 10

Le dossier MBED contient le code source de l'application de simulateur codée sur MBED
Il est conseillé de compiler ce fichier pour LPC1768 sur le compilateur en ligne de MBED

Le dossier RPI_CODE contient le code qui doit tourner sur Raspberry Pi 3 avec Xenomai 3.0.3
Le code est en C++ et les bibliothèques en C. 