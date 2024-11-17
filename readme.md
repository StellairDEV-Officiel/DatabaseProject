# DatabaseProject
DatabaseProject est une mini base de données développée en langage C, conçue pour gérer des tables et leurs enregistrements.

Crée par Zacharie ROGER en classe 3SI1.

## Structure du projet
Le projet est organisé comme suit :

Répertoire src : Contient les fichiers source .c suivants :

* main.c
* command.c
* database.c
* records_btree.c
* table_btree.c
* storage.c

Répertoire include : Contient les fichiers d'en-tête .h correspondants à chaque fichier source du répertoire src.

## Fonctionnalités

#### Stockage des données : Chaque table est associée à un fichier nommé `nomdelatable_data.dat` pour le stockage de ses données. Les informations relatives aux tables sont centralisées dans le fichier tables.dat.

#### Système d'indexation : Le projet implémente un système d'index basé sur des arbres B (B-trees) pour optimiser l'accès et la gestion des enregistrements. Les fichiers `records_btree.c` et `table_btree.c` gèrent respectivement les index des enregistrements et des tables.

#### Gestion des offsets : L'accès aux données utilise un système d'offsets pour localiser précisément les enregistrements dans les fichiers de données.

### Compilation

Pour compiler le projet, utilisez le fichier makefile fourni.

### Utilisation

Après compilation, exécutez le binaire généré pour interagir avec la base de données. La liste des commandes est disponible en tappant `.help` .
