# OceanDepth

## Description du Projet

OceanDepth est un jeu d'exploration sous-marine en mode texte developpe en langage C. Le joueur incarne un plongeur temeraire qui explore les profondeurs oceaniques, affronte des creatures marines redoutables, decouvre des tresors et gere ses ressources vitales (vie, oxygene, fatigue).

## Fonctionnalites Implementees

### Systeme de Combat
- Combat contre 1 a 4 creatures marines simultanees
- 5 types de creatures : Kraken, Requin-Tigre, Meduse Bleue, Poisson-Epee, Crabe Geant
- Effets speciaux uniques pour chaque creature
- Gestion de la fatigue (limite les attaques : 3/2/1 par tour)
- Consommation d'oxygene a chaque action
- Systeme d'alertes critiques (oxygene <= 10)

### Gestion des Ressources
- Points de vie (PV)
- Oxygene (diminue a chaque action, critique si trop bas)
- Fatigue (limite le nombre d'attaques par tour)
- Perles (monnaie du jeu)

### Systeme de Progression
- Gain d'experience (XP) apres chaque combat
- Systeme de niveaux
- Statistiques de progression (creatures tuees, boss vaincus)

### Equipements et Objets
- Harpons avec differentes puissances (Rouille, Acier, Electrique, Laser)
- Combinaisons offrant defense et bonus (Neoprene, Renforcee, Profondeur, Titanium)
- Consommables : Capsule d'oxygene, Trousse de soin, Stimulant, Antidote
- Inventaire limite a 8 objets

### Competences Aquatiques
- Apnee Prolongee (+20 oxygene, cooldown 4 tours)
- Decharge Electrique (degats de zone, 18 oxygene)
- Communication Marine (pacifie une creature, 8 oxygene)
- Tourbillon Aquatique (reduit vitesse ennemis, 22 oxygene)

### Cartographie et Exploration
- Systeme de carte par zones avec profondeur croissante
- Types de tuiles : Combat, Tresor, Marchand, Boss, Vide
- Navigation libre dans les zones
- Progression par zones avec boss a vaincre

### Commerce
- Marchand accessible dans certaines zones
- Achat d'equipements et de consommables avec les perles

### Sauvegarde
- Systeme de sauvegarde/chargement complet
- Format structure OCEANDEPTHS_SAVE_V1
- Sauvegarde de toutes les donnees du joueur et de la carte
- Fichiers stockes dans code/saves/

## Structure du Projet

```
oceandepths/
├── identifiants.txt      # Identifiants du groupe
├── README.md             # Ce fichier
├── PROGRESSION.md        # Suivi des etapes realisees
├── instructions.md       # Consignes du projet
├── code/
│   ├── Makefile         # Compilation automatique
│   ├── config.txt       # Configuration du jeu
│   ├── src/             # Fichiers sources (.c)
│   │   ├── main.c
│   │   ├── combat.c
│   │   ├── creature.c
│   │   ├── joueur.c
│   │   ├── competences.c
│   │   ├── map.c
│   │   ├── save.c
│   │   ├── marchand.c
│   │   ├── equipement.c
│   │   ├── weapon.c
│   │   ├── consommable.c
│   │   ├── game.c
│   │   ├── menu.c
│   │   ├── config.c
│   │   └── utils.c
│   ├── include/         # Fichiers headers (.h)
│   │   ├── struct.h
│   │   ├── combat.h
│   │   ├── creature.h
│   │   ├── joueur.h
│   │   ├── competences.h
│   │   ├── map.h
│   │   ├── save.h
│   │   ├── marchand.h
│   │   ├── equipement.h
│   │   ├── weapon.h
│   │   ├── consommable.h
│   │   ├── game.h
│   │   ├── menu.h
│   │   ├── config.h
│   │   ├── tab.h
│   │   ├── utils.h
│   │   └── include.h
│   ├── obj/             # Fichiers objets (generes)
│   └── saves/           # Sauvegardes du jeu
└── main.exe             # Executable (genere)
```

## Compilation

### Prerequis
- Compilateur GCC (MinGW sous Windows)
- Make

### Commandes de Compilation

```bash
# Se placer dans le dossier code/
cd code/

# Compilation complete
make

# Nettoyage des fichiers de compilation
make clean

# Recompilation complete (clean + all)
make re
```

L'executable main.exe sera cree a la racine du projet.

## Execution

### Windows
```bash
./main.exe
```

### Linux/Mac
```bash
chmod +x main.exe
./main.exe
```

## Utilisation

### Menu Principal
1. Nouvelle partie : Demarre une nouvelle aventure
2. Charger une partie : Charge une sauvegarde existante (si disponible)
3. Quitter : Ferme le jeu

### En Jeu
- Explorez les differentes zones oceaniques
- Affrontez des creatures marines
- Gerez vos ressources (vie, oxygene, fatigue)
- Collectez des perles pour acheter des equipements
- Utilisez vos competences aquatiques strategiquement
- Sauvegardez votre progression

### Combat
- Attaquer : Utilisez votre harpon (consomme fatigue et oxygene)
- Competence : Utilisez une competence marine speciale
- Objet : Consommez un objet de l'inventaire
- Terminer le tour : Passez au tour suivant

## Tests et Validation

### Test de Compilation
```bash
cd code/
make clean && make
```

### Test de Fuites Memoire (Linux/Mac)
```bash
valgrind --leak-check=full ./main.exe
```

### Test de Fonctionnement
- Creation de personnage
- Combat contre differentes creatures
- Utilisation de competences
- Utilisation d'objets
- Sauvegarde et chargement
- Navigation dans la carte
- Commerce avec le marchand

## Difficultes Rencontrees et Solutions

### Gestion de la Memoire Dynamique
- Probleme : Allocation dynamique pour la carte avec zones multiples
- Solution : Systeme de zones chainees avec liberation recursive

### Systeme de Combat Multi-Creatures
- Probleme : Gerer l'ordre d'attaque et les effets speciaux simultanes
- Solution : Tri par vitesse et systeme de flags pour les effets actifs

### Sauvegarde de la Carte
- Probleme : Format de sauvegarde pour les structures complexes
- Solution : Format texte structure avec sections dediees

## Ameliorations Futures Possibles

- Interface graphique (ncurses)
- Systeme de quetes
- Plus de types de creatures
- Evenements aleatoires (tempetes, courants)
- Mode multijoueur local
- Animations ASCII avancees
- Sons et musiques

## Auteurs

Voir le fichier identifiants.txt

## Licence

Projet pedagogique - 2025

---

Bon plongeon dans les profondeurs !
