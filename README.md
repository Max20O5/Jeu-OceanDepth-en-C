# OceanDepth 🤿🪙🐙
## 📖 Description du Projet
OceanDepth est un jeu d'aventure textuel en C où l'on incarne un plongeur explorant les abysses. L'objectif est de survivre, de combattre des créatures marines, d'acheter de l'équipement et de découvrir des trésors enfouis.

Ce projet met en œuvre un moteur de jeu entièrement "data-driven" : toutes les entités (créatures, armes, compétences, consommables) et la configuration de la carte sont chargées depuis des fichiers de configuration externes.

## 🚀 Lancer le projet
### Prérequis
Un compilateur C (comme gcc)

L'utilitaire Make

Compilation et Lancement
Toutes les commandes sont à exécuter depuis le dossier racine du projet

```
# Pour compiler le projet (place les fichiers .o dans code/obj/)
make

# Pour lancer le jeu (l'exécutable est à la racine)
# Sur Windows
.\main.exe

# Sur Linux/macOS
./main.exe

# Pour nettoyer les fichiers compilés
make clean
```

## 💾 Liste des fonctionnalités

Ce projet implémente les fonctionnalités suivantes :

* **Chargement de donnée** : Chargement de toutes les données de jeu (Armes, Créatures, Compétences, Consommables) depuis des fichiers `.cfg` dédiés dans le dossier `/config`.
* **Système de Carte Procédurale (`map.c`)** :
    * Génération de zones sous forme de grille 2D.
    * Types de tuiles (Combat, Trésor, Marchand, Boss, Grotte de sauvegarde).
    * Configuration de la carte (taille, probabilités) chargée depuis `config/config.cfg`.
    * Déplacement du joueur et exploration.
* **Système de Sauvegarde (`save.c`)** :
    * Sauvegarde et chargement de l'intégralité de la progression du joueur et de l'état de la carte.
    * Sauvegarde automatique dans le dossier `/saves`.
* **Combat Tactique (`combat.c`)** :
    * Combat au tour par tour.
    * Gestion de PV, Oxygène et Fatigue.
    * Le niveau de fatigue (0-5) limite le nombre d'attaques par tour.
* **Système de Compétences (`competence.c`)** :
    * 4 compétences de joueur avec coûts (Oxygène, Fatigue) et Cooldowns.
    * Logique de ciblage pour les sorts mono-cibles et de zone (AOE).
* **Système d'Effets (`effects.c`)** :
    * Gestion des effets de statut (Brûlure, Paralysie, Charme, Ralentissement) sur le joueur et les ennemis.
    * Gestion des effets instantanés (VolDeVie, Force).
* **Marchand (`marchand.c`)** :
    * Boutique pour acheter des consommables.
    * Achat d'armes (remplace l'arme actuelle).
    * Achat de compétences (remplit un slot vide).
* **Système de Progression (`joueur.c`)** :
    * Système de Niveau et d'Expérience (XP).
    * Montée de niveau avec amélioration des statistiques (PV max, O2 max).

[Voir la progression détaillée](PROGRESSION.md)
#

```
OceanDepth/
│   .gitignore
│   2025 - OceanDepth.pdf
│   identifiants.txt
│   instructions.md
│   main.exe                  
│   PROGRESSION.md
│   README.md
│
├───code/
│   │   Makefile
│   │
│   ├───include/
│   │       combat.h
│   │       competence.h
│   │       config.h
│   │       consommable.h
│   │       creature.h
│   │       effects.h
│   │       equipement.h
│   │       game.h
│   │       include.h
│   │       joueur.h
│   │       loader.h
│   │       map.h
│   │       marchand.h
│   │       menu.h
│   │       save.h
│   │       struct.h
│   │       tab.h
│   │       utils.h
│   │       weapon.h
│   │
│   ├───obj/
│   │       (fichiers .o compilés)
│   │
│   └───src/
│           combat.c
│           competence.c
│           config.c
│           consommable.c
│           creature.c
│           effects.c
│           equipement.c
│           game.c
│           joueur.c
│           loader.c
│           main.c
│           map.c
│           marchand.c
│           menu.c
│           save.c
│           utils.c
│           weapon.c
│
├───config/
│       config.cfg
│       consumables.cfg
│       creatures.cfg
│       equipements.cfg         
│       skills.cfg
│       weapons.cfg
│
└───saves/
        oceandepth_save.txt
```
OceanDepth 🤿🪙🐙
📖 Description du Projet
OceanDepth est un jeu d'aventure textuel en C où l'on incarne un plongeur explorant les abysses. L'objectif est de survivre, de combattre des créatures marines, d'acheter de l'équipement et de découvrir des trésors enfouis.

Ce projet met en œuvre un moteur de jeu entièrement "data-driven" : toutes les entités (créatures, armes, compétences, consommables) et la configuration de la carte sont chargées depuis des fichiers de configuration externes.

```
OceanDepth/
│   .gitignore
│   2025 - OceanDepth.pdf
│   identifiants.txt
│   instructions.md
│   main.exe                  
│   PROGRESSION.md
│   README.md
│
├───code/
│   │   Makefile
│   │
│   ├───include/
│   │       combat.h
│   │       competence.h
│   │       config.h
│   │       consommable.h
│   │       creature.h
│   │       effects.h
│   │       equipement.h
│   │       game.h
│   │       include.h
│   │       joueur.h
│   │       loader.h
│   │       map.h
│   │       marchand.h
│   │       menu.h
│   │       save.h
│   │       struct.h
│   │       tab.h
│   │       utils.h
│   │       weapon.h
│   │
│   ├───obj/
│   │       (fichiers .o compilés)
│   │
│   └───src/
│           combat.c
│           competence.c
│           config.c
│           consommable.c
│           creature.c
│           effects.c
│           equipement.c
│           game.c
│           joueur.c
│           loader.c
│           main.c
│           map.c
│           marchand.c
│           menu.c
│           save.c
│           utils.c
│           weapon.c
│
├───config/
│       config.cfg
│       consumables.cfg
│       creatures.cfg
│       equipements.cfg         
│       skills.cfg
│       weapons.cfg
│
└───saves/
        oceandepth_save.txt
```

│   instructions.md
│   main.exe                  
│   PROGRESSION.md
│   README.md
│
├───code/
│   │   Makefile
│   │
│   ├───include/
│   │       combat.h
│   │       competence.h
│   │       config.h
│   │       consommable.h
│   │       creature.h
│   │       effects.h
│   │       equipement.h
│   │       game.h
│   │       include.h
│   │       joueur.h
│   │       loader.h
│   │       map.h
│   │       marchand.h
│   │       menu.h
│   │       save.h
│   │       struct.h
│   │       tab.h
│   │       utils.h
│   │       weapon.h
│   │
│   ├───obj/
│   │       (fichiers .o compilés)
│   │
│   └───src/
│           combat.c
│           competence.c
│           config.c
│           consommable.c
│           creature.c
│           effects.c
│           equipement.c
│           game.c
│           joueur.c
│           loader.c
│           main.c
│           map.c
│           marchand.c
│           menu.c
│           save.c
│           utils.c
│           weapon.c
│
├───config/
│       config.cfg
│       consumables.cfg
│       creatures.cfg
│       equipements.cfg         
│       skills.cfg
│       weapons.cfg
│
└───saves/
        oceandepth_save.txt
```



