Implémentation d'un algorithme d'évolution pour Core-War
Gaël Halnaut, Florian Dayre, Alex Fournier, Frederic Chipot, Louis Duplantier

version 1 (09/04/2021)

Utilisation sous Windows:
La compilation s'effectue sous l'IDE Microsoft Visual Studio Community 2019, que vous pouvez télécharger ici :
https://visualstudio.microsoft.com/fr/

Suivre l'installation desktop c++.
Une fois l'IDE téléchargé ainsi que les sources du projet récuperées, il faut construire le projet en executant core-wars.sln

Il suffit de sélectionner le module Evolution algorithm, clique droit et définir en tant que projet de démarrage.
Puis changer le mode d'exécution de debug à release. Puis exécuter, ou f5. 


Note : il est parfois nécessaire de lancer deux fois un projet pour qu'il soit correctement compilé car Visual Studio 
ne trouve des fois pas la bibliothèque Exmars qu'il vient de compiler (mais y arrive au deuxième essai)

Une fois l'execution terminée (10minutes environ avec les paramètres de base), les guerriers générés se trouvent dans data/Warriors/Generated/*.red 

################################################################

Consignes compilation UNIX - LINUX 
avoir pour répertoire courant core-war puis utiliser les commandes 
$ make setup
$ make 
$ cd src/evolution-algorithm
$ ./evolution
ou 
$ OMP_NUM_THREADS=X ./evolution
où X = nombre de threads que vous souhaitez utiliser (à sélectionner judicieusement
selon les caracteristiques de votre machine).
Il est possible que la commande setaway ne fonctionne pas si vous avez utiliser
make clean avant ou s'il manque une ses cibles. Dans ce cas, il faudra supprimer
manuellement tel qu'indiquer à partir de la racine core-war avec la commande make setaway
Puis vous pourrez utiliser la commande make setup à nouveau.