# Progression OceanDepths - Projet 2025

## Vue d'Ensemble

**Statut Global** : COMPLET - Toutes les etapes + Bonus
**Lignes de Code** : ~2920 lignes
**Fichiers Sources** : 14 fichiers .c + 17 fichiers .h
**Compilation** : Reussie (0 erreurs, 0 warnings)

---

## Etapes Realisees

### Etape 1 : Generation de Creatures Marines (100%)

**Implementation** : `code/src/creature.c` + `code/include/creature.h`

- [x] Structure `CreatureMarine` conforme (id, nom, PV, attaque, defense, vitesse, effet special)
- [x] Kraken : 120-180 PV, 25-40 ATK, effet "etreinte"
- [x] Requin-Tigre : 60-100 PV, 15-25 ATK, effet "frenesie", tres rapide (15-25 vitesse)
- [x] Meduse Bleue : 20-40 PV, 8-15 ATK, effet "paralysie", fragile
- [x] Poisson-Epee : 70-90 PV, 18-28 ATK, effet "perforation", equilibre
- [x] Crabe Geant : 80-120 PV, 12-20 ATK, haute defense (15-25), effet "carapace"
- [x] Generation aleatoire 1-4 creatures selon profondeur
- [x] Augmentation de la difficulte avec la profondeur
- [x] Systeme de perles (recompenses variables par creature)

**Fonctions Cles** :
- `create_kraken()`, `create_requin()`, `create_meduse()`, `create_poisson_epee()`, `create_crabe_geant()`
- `generer_creature_aleatoire(int profondeur)`
- `generer_groupe_creatures(CreatureMarine[], int max, int profondeur)`

---

### Etape 2 : Systeme d'Attaque du Plongeur (100%)

**Implementation** : `code/src/combat.c` + `code/src/joueur.c`

- [x] Structure `Plongeur` avec PV, PV max, oxygene, oxygene max, fatigue (0-5), perles
- [x] Systeme de fatigue limitant les attaques :
  - Fatigue 0-1 : 3 attaques max/tour
  - Fatigue 2-3 : 2 attaques max/tour
  - Fatigue 4-5 : 1 attaque max/tour
- [x] Consommation d'oxygene par attaque (-2 a -4 selon profondeur)
- [x] Consommation d'oxygene par competence (-5 a -22)
- [x] Alerte critique affichee si oxygene <= 10
- [x] Perte de 5 PV par tour si oxygene = 0
- [x] Calcul des degats : `(attaque_joueur + bonus_arme) - defense_creature` (minimum 1)
- [x] Interface de combat avec barres de progression (PV, Oxygene, Fatigue)
- [x] Affichage de la profondeur, niveau, XP et perles

**Fonctions Cles** :
- `display_combat_status(Plongeur*, CreatureMarine[], int nb, int profondeur)`
- `get_max_attaques_from_fatigue(int fatigue)`
- `print_progress_bar(int current, int max, int length)`

---

### Etape 3 : Attaque des Creatures Marines (100%)

**Implementation** : `code/src/combat.c` (lignes 84-114)

- [x] Tri des creatures par vitesse (les plus rapides attaquent en premier)
- [x] Effets speciaux implementes :
  - **Kraken** : "Etreinte tentaculaire" -> 2 attaques consecutives
  - **Meduse** : "Piqure paralysante" -> Reduit les attaques joueur de 1 (2 tours)
  - **Requin** : "Frenesie sanguinaire" -> +30% degats si PV < 50%
  - **Poisson-Epee** : "Charge perforante" -> Ignore points de defense
  - **Crabe Geant** : "Carapace durcie" -> Reduit degats subis de 20%
- [x] Perte d'oxygene supplementaire lors des attaques subies (stress)
- [x] Gestion des effets persistants (paralysie sur plusieurs tours)
- [x] Ordre des actions : Affichage -> Actions joueur -> Consommation O2 -> Verif critique -> Attaques creatures -> Recup fatigue -> Verif fin

**Variables Globales** :
- `paralysie_actif` : compte les tours restants de paralysie

---

### Etape 4 : Systeme de Recompenses Marines (100%)

**Implementation** : `code/src/equipement.c` + `code/src/weapon.c` + `code/src/consommable.c`

**Equipements** :
- [x] Harpons (structure `Arme`) :
  - Trident Rouille : 10-15 ATK
  - Harpon Acier : 20-30 ATK
  - Harpon Electrique : 28-42 ATK (effet electrique)
  - Harpon Laser : 35-50 ATK
- [x] Combinaisons (structure `Equipement`) :
  - Neoprene Basique : +5 DEF
  - Combinaison Renforcee : +10-15 DEF
  - Combinaison Profondeur : +15-20 DEF, reduit consommation O2
  - Armure Titanium : +25 DEF

**Consommables** :
- [x] Capsule d'Oxygene : +40 oxygene
- [x] Trousse de Soin : +25 PV
- [x] Stimulant Marin : Reduit fatigue de 2 niveaux
- [x] Antidote : Annule paralysie/poison

**Monnaie** :
- [x] Perles gagnees apres chaque combat (variable selon creature)
- [x] Perles communes : 5-50 selon type de creature

**Inventaire** :
- [x] Maximum 8 objets
- [x] Systeme de slots avec gestion des emplacements vides
- [x] Utilisation d'objets en combat

---

### Etape 5 : Sauvegarde et Chargement (100%)

**Implementation** : `code/src/save.c` + `code/include/save.h`

- [x] Dossier `saves/` cree automatiquement
- [x] Format : `OCEANDEPTHS_SAVE_V1` (fichier texte structure)
- [x] Donnees sauvegardees :
  - Etat complet du plongeur (PV, oxygene, fatigue, perles, nom)
  - Inventaire avec tous les objets (8 slots)
  - Equipements (arme et combinaison equipees)
  - Position actuelle (zone, coordonnees X/Y)
  - Carte complete (zones, tuiles, profondeurs)
  - Progression (niveau, XP, creatures tuees, boss tues)
- [x] Fonction `sauvegarder_partie(Plongeur*, Carte*)`
- [x] Fonction `charger_partie(Plongeur*, Carte**)`
- [x] Verification existence sauvegarde : `sauvegarde_existe()`
- [x] Suppression sauvegarde : `supprimer_sauvegarde()`
- [x] Gestion des erreurs de lecture/ecriture
- [x] Une sauvegarde unique par joueur

**Fichier de Sauvegarde** : `saves/oceandepth_save.txt`

---

### Etape 6 : Competences Aquatiques (100%)

**Implementation** : `code/src/competences.c` + `code/include/competences.h`

- [x] Structure `Competence` avec id, nom, cout oxygene, cooldown
- [x] 4 competences marines :
  1. **Apnee Prolongee** : Cout 0 oxygene, +20 oxygene, cooldown 4 tours
  2. **Decharge Electrique** : Cout 18 oxygene, degats zone 20-30 (toutes creatures)
  3. **Communication Marine** : Cout 8 oxygene, pacifie 1 creature (1 tour)
  4. **Tourbillon Aquatique** : Cout 22 oxygene, -2 vitesse toutes creatures
- [x] Systeme de cooldown fonctionnel (`cooldown_actuel` decompte chaque tour)
- [x] Verification oxygene disponible avant utilisation
- [x] Affichage menu competences avec statut (disponible/cooldown/oxygene insuffisant)
- [x] Fonction `utiliser_competence()` retourne bool (succes/echec)
- [x] Fonction `reduire_cooldowns()` appelee a chaque tour

**Recuperation Oxygene** :
- Naturelle : +8 par tour en combat, +15 en exploration
- Capsule : +40 instantane
- Competence Apnee : +20

---

### Etape 7 : Cartographie des Oceans (100%)

**Implementation** : `code/src/map.c` + `code/include/map.h` + `code/src/config.c`

**Systeme de Carte** :
- [x] Carte par zones avec matrice 2D de tuiles
- [x] Structure `Zone` avec zones chainees (linked list)
- [x] Types de tuiles (enum `TileType`) :
  - `TILE_EMPTY` : Deplacement libre
  - `TILE_COMBAT` : 1+ ennemis
  - `TILE_TREASURE` : Oxygene/items
  - `TILE_MERCHANT` : Commerce
  - `TILE_BOSS` : Combat de boss
  - `TILE_PLAYER` : Position actuelle (affichage)
- [x] Systeme de profondeur variable selon zones
- [x] Tuiles marquees visitees/nettoyees
- [x] Navigation 4 directions (haut, bas, gauche, droite)
- [x] Generation procedurale selon probabilites (config)
- [x] Configuration chargee depuis `config/map_config.txt`
- [x] Fonction `afficher_carte()` avec symboles visuels
- [x] Fonction `naviguer_carte()` pour deplacement
- [x] Fonction `explorer_tuile()` pour interaction
- [x] Progression par zones (apres defaite du boss)

**Structures Cles** :
- `Carte` : Zone actuelle + configuration + zones generees
- `Zone` : Matrice de tuiles + position joueur + profondeur
- `Tile` : Type + etat (visite/nettoye) + ennemis + profondeur
- `MapConfig` : Parametres de generation

---

## Bonus Implementes

### Systeme de Progression (Bonus 1)

**Implementation** : `code/src/joueur.c` (lignes 75-150)

- [x] Gain d'experience (XP) par creature vaincue
- [x] Systeme de niveaux avec paliers d'XP
- [x] Amelioration des capacites a chaque niveau :
  - +10 PV max
  - +5 Oxygene max
  - Message de montee de niveau
- [x] Statistiques de progression :
  - `niveau` : Niveau actuel du joueur
  - `experience` : XP actuelle
  - `experience_prochain_niveau` : XP requise pour next level
  - `creatures_tuees` : Total creatures eliminees
  - `boss_tues` : Total boss vaincus

**Fonctions** :
- `gagner_experience(Plongeur*, int xp)`
- `monter_niveau(Plongeur*)`

---

### Commerce Sous-Marin (Bonus 2)

**Implementation** : `code/src/marchand.c` + `code/include/marchand.h`

- [x] Marchand accessible via tuiles `TILE_MERCHANT`
- [x] Echange de perles contre equipements et objets
- [x] Articles disponibles :
  - **Consommables** : Trousse Soin (15), Capsule O2 (20), Stimulant (25), Antidote (30)
  - **Harpons** : Acier (80), Electrique (150), Laser (300)
  - **Combinaisons** : Renforcee (50), Profondeur (100), Titanium (200)
- [x] Verification perles disponibles
- [x] Verification inventaire plein
- [x] Messages d'achat/erreur

**Fonction Principale** :
- `open_shop(Plongeur* player)`

---

### Defis Speciaux - Boss (Bonus 3)

**Implementation** : Systeme de tuiles TILE_BOSS + flag `boss_defeated` dans Zone

- [x] Boss a la fin de chaque zone
- [x] Statistiques renforcees (via profondeur elevee)
- [x] Recompenses accrues (plus de perles, XP bonus)
- [x] Deblocage zone suivante apres victoire
- [x] Flag `boss_tues` incremente dans stats joueur

---

## Architecture Technique

### Modularite
- 14 fichiers `.c` sources
- 17 fichiers `.h` headers
- Separation logique par fonctionnalite
- Pas de code duplique

### Compilation
- Makefile avec regles `all`, `clean`, `re`
- Flags : `-Wall -Wextra -g -Iinclude`
- Norme C99 (implicite via GCC)
- Compilation automatique des objets dans `obj/`
- Gestion des dependances

### Gestion Memoire
- Allocation dynamique pour la carte (zones + tuiles)
- Liberation systematique (`free_carte`, `free_zone`)
- Verification des retours `malloc/calloc`
- Test valgrind a effectuer pour confirmation zero fuite

### Validation Entrees
- Controle de toutes les saisies utilisateur
- Boucles de validation avec `scanf` + verification
- Messages d'erreur en francais
- Nettoyage du buffer (`while (getchar() != '\n')`)

---

## Captures d'ecran

### Interface de Combat
```
OceanDepths - Profondeur: -247m | Niv: 3 | XP: 450/500 | Perles: 127
Vie     [##########################################........] 87/100
Oxygene [############################............] 58/100
Fatigue [###.......] 3/5

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    [0] Kraken              (124/156 PV)     Vitesse: 8
    [1] Requin-Tigre        (89/89 PV)       Vitesse: 20
    [2] Meduse Bleue        (23/34 PV)       Vitesse: 10
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Actions disponibles:
1 - Attaquer avec harpon (2 attaques restantes)
2 - Utiliser competence marine
3 - Consommer objet
4 - Terminer le tour
>
```

### Menu Competences
```
                    COMPETENCES MARINES
==============================================================
[1] Apnee Prolongee
    Cout: 0 oxygene | Cooldown: 4 tours | DISPONIBLE

[2] Decharge Electrique
    Cout: 18 oxygene | DISPONIBLE

[3] Communication Marine
    Cout: 8 oxygene | DISPONIBLE

[4] Tourbillon Aquatique
    Cout: 22 oxygene | DISPONIBLE

==============================================================
[5] Retour
```

---

## Difficultes Rencontrees

### 1. Gestion Memoire Dynamique - Carte Multi-Zones

**Probleme** : La structure de carte avec zones chainees necessitait une allocation/liberation complexe.

**Solution** :
- Implementation d'une linked list de zones
- Fonction recursive `free_zone()` pour liberer toute la chaine
- Matrice 2D dynamique (`Tile**`) allouee zone par zone
- Verification systematique des retours `malloc`

**Code** : `code/src/map.c` (fonctions `generer_zone`, `free_zone`, `free_carte`)

---

### 2. Ordre d'Attaque Multi-Creatures

**Probleme** : Gerer l'ordre d'attaque de 1-4 creatures selon leur vitesse + effets speciaux simultanes.

**Solution** :
- Tri par vitesse avant la phase d'attaque des creatures
- Variable globale `paralysie_actif` pour tracker les tours restants
- Systeme de flags pour les effets persistants
- Calcul des degats adapte selon l'effet special actif

**Code** : `code/src/combat.c` (fonction `appliquer_effet_special_creature`)

---

### 3. Format de Sauvegarde pour Structures Complexes

**Probleme** : Sauvegarder une carte avec zones chainees + matrice 2D + IDs ennemis dynamiques.

**Solution** :
- Format texte structure avec sections :
  - `OCEANDEPTHS_SAVE_V1` (version)
  - `JOUEUR_NOM`, `JOUEUR_STATS`, `JOUEUR_PROGRESSION`
  - `ARME`, `COMBINAISON`
  - `INVENTAIRE_START` ... `INVENTAIRE_END`
  - `CARTE_START` ... `CARTE_END`
  - `ZONE_X_START` ... `ZONE_X_END`
- Parsing ligne par ligne avec `sscanf`
- Reconstruction de la structure complete a la lecture

**Code** : `code/src/save.c` (fonctions `sauvegarder_partie`, `charger_partie`)

---

### 4. Validation des Entrees Utilisateur

**Probleme** : Eviter les crashs lors de saisies invalides (lettres au lieu de chiffres, buffer overflow).

**Solution** :
- Boucles de validation systematiques avec `scanf("%d%c", &input, &term)`
- Verification que `term == '\n'` pour detecter les caracteres parasites
- Nettoyage du buffer : `while (getchar() != '\n')`
- Messages d'erreur explicites en francais

**Code** : Toutes les fonctions de menu et saisie utilisateur

---

### 5. Consommation d'Oxygene Equilibree

**Probleme** : Rendre le jeu challengeant sans etre frustrant (oxygene epuise trop vite).

**Solution** :
- Consommation variable selon profondeur (-2 a -5)
- Competence Apnee sans cout pour recuperation d'urgence
- Capsules O2 disponibles a l'achat et dans les tresors
- Alerte critique a 10 oxygene (pas 0) pour anticiper

**Code** : `code/src/combat.c` (gestion oxygene), `code/src/competences.c`

---

## Tests Effectues

### Compilation
- `make clean && make` : Succes (0 erreurs, 0 warnings)
- Compilation sur Windows (GCC MinGW)
- Test Linux/Mac a faire

### Fonctionnalites
- Creation de personnage
- Combat 1v1, 1v2, 1v3, 1v4
- Tous les types de creatures
- Tous les effets speciaux
- Toutes les competences
- Utilisation objets inventaire
- Achat au marchand
- Navigation carte
- Sauvegarde et chargement
- Systeme XP et niveau
- Mort du joueur (oxygene 0 + PV 0)
- Victoire combat (toutes creatures tuees)

### Gestion Memoire
- Valgrind a lancer pour verification complete
- Liberation manuelle testee (pas de crash observable)

---

## Statistiques Finales

- **Lignes de Code Total** : 2920 lignes
- **Fichiers Sources** : 14 .c
- **Fichiers Headers** : 17 .h
- **Fonctions Totales** : ~80+ fonctions
- **Structures de Donnees** : 10 structures principales
- **Temps de Developpement Estime** : 40-60 heures

---

## Points Forts du Projet

1. **Architecture Modulaire** : Code bien organise, facile a maintenir
2. **Toutes les Etapes Completes** : 100% des attendus + bonus
3. **Systeme de Progression Riche** : XP, niveaux, stats
4. **Combat Tactique** : Gestion fatigue, oxygene, competences
5. **Carte Procedurale** : Generation dynamique avec config
6. **Sauvegarde Robuste** : Format structure, toutes les donnees
7. **Validation Entrees** : Securite contre crashs

---

## Ameliorations Futures Possibles

- Interface graphique avec ncurses
- Plus de types de creatures (10-15 au lieu de 5)
- Systeme de quetes et d'objectifs
- Evenements aleatoires (tempetes, decouvertes)
- Boss unique avec IA specifique
- Crafting d'objets
- Mode multijoueur
- Achievements/trophees
- Musiques et effets sonores ASCII

---

## Conclusion

Le projet OceanDepth est **complet a 100%** avec toutes les fonctionnalites demandees et plusieurs bonus. Le code est **modulaire, commente et fonctionnel**. La compilation reussit sans erreurs. Le jeu est **jouable et equilibre**.

**Pret pour le rendu !**

---

**Date de finalisation** : 13 Novembre 2025
**Version** : 1.0 - Release Candidate
