#include "creature.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Kraken : Points de vie élevés (120-180), attaque puissante (25-40)
CreatureMarine create_kraken() {
    CreatureMarine kraken;
    kraken.id = 1;
    strcpy(kraken.nom, "Kraken");
    kraken.points_de_vie_max = randomNumber(120, 180);
    kraken.points_de_vie_actuels = kraken.points_de_vie_max;
    kraken.attaque_minimale = 25;
    kraken.attaque_maximale = 40;
    kraken.defense = randomNumber(10, 15);
    kraken.vitesse = randomNumber(5, 10);
    kraken.perles_min = 30;
    kraken.perles_max = 50;
    strcpy(kraken.effet_special, "etreinte");
    kraken.est_vivant = true;
    kraken.zone = 0;  // Zone définie dynamiquement
    return kraken;
}

// Requin : Rapide, attaques multiples (60-100 PV, 15-25 ATK)
CreatureMarine create_requin() {
    CreatureMarine requin;
    requin.id = 2;
    strcpy(requin.nom, "Requin-Tigre");
    requin.points_de_vie_max = randomNumber(60, 100);
    requin.points_de_vie_actuels = requin.points_de_vie_max;
    requin.attaque_minimale = 15;
    requin.attaque_maximale = 25;
    requin.defense = randomNumber(3, 8);
    requin.vitesse = randomNumber(15, 25); // Très rapide
    requin.perles_min = 15;
    requin.perles_max = 30;
    strcpy(requin.effet_special, "frenesie");
    requin.est_vivant = true;
    requin.zone = 0;  // Zone définie dynamiquement
    return requin;
}

// Méduse : Fragile mais paralysante (20-40 PV, 8-15 ATK)
CreatureMarine create_meduse() {
    CreatureMarine meduse;
    meduse.id = 3;
    strcpy(meduse.nom, "Meduse Bleue");
    meduse.points_de_vie_max = randomNumber(20, 40);
    meduse.points_de_vie_actuels = meduse.points_de_vie_max;
    meduse.attaque_minimale = 8;
    meduse.attaque_maximale = 15;
    meduse.defense = randomNumber(1, 3);
    meduse.vitesse = randomNumber(8, 12);
    meduse.perles_min = 5;
    meduse.perles_max = 15;
    strcpy(meduse.effet_special, "paralysie");
    meduse.est_vivant = true;
    meduse.zone = 0;  // Zone définie dynamiquement
    return meduse;
}

// Poisson-Épée : Équilibré (70-90 PV, 18-28 ATK)
CreatureMarine create_poisson_epee() {
    CreatureMarine poisson;
    poisson.id = 4;
    strcpy(poisson.nom, "Poisson-Epee");
    poisson.points_de_vie_max = randomNumber(70, 90);
    poisson.points_de_vie_actuels = poisson.points_de_vie_max;
    poisson.attaque_minimale = 18;
    poisson.attaque_maximale = 28;
    poisson.defense = randomNumber(5, 10);
    poisson.vitesse = randomNumber(12, 18);
    poisson.perles_min = 15;
    poisson.perles_max = 25;
    strcpy(poisson.effet_special, "perforation");
    poisson.est_vivant = true;
    poisson.zone = 0;  // Zone définie dynamiquement
    return poisson;
}

// Crabe Géant : Défensif (80-120 PV, 12-20 ATK, haute défense)
CreatureMarine create_crabe_geant() {
    CreatureMarine crabe;
    crabe.id = 5;
    strcpy(crabe.nom, "Crabe Geant");
    crabe.points_de_vie_max = randomNumber(80, 120);
    crabe.points_de_vie_actuels = crabe.points_de_vie_max;
    crabe.attaque_minimale = 12;
    crabe.attaque_maximale = 20;
    crabe.defense = randomNumber(15, 25); // Haute défense
    crabe.vitesse = randomNumber(3, 7); // Lent
    crabe.perles_min = 20;
    crabe.perles_max = 35;
    strcpy(crabe.effet_special, "carapace");
    crabe.est_vivant = true;
    crabe.zone = 0;  // Zone définie dynamiquement
    return crabe;
}

// Générer une créature aléatoire selon la profondeur
CreatureMarine generer_creature_aleatoire(int profondeur) {
    int type_creature;
    int prof_abs = abs(profondeur);

    // Plus on descend, plus les créatures dangereuses apparaissent
    if (prof_abs == 0) { // Surface
        type_creature = rand() % 2 + 3; // Méduse ou Poisson-Épée (3-4)
    } else if (prof_abs <= 500) { // Zone facile (0-500m)
        type_creature = rand() % 3 + 2; // Requin, Méduse ou Poisson (2-4)
    } else if (prof_abs <= 1500) { // Zone moyenne (500-1500m)
        type_creature = rand() % 4 + 2; // Requin, Méduse, Poisson, Crabe (2-5)
    } else { // Zone difficile (1500m+)
        type_creature = rand() % 5 + 1; // Toutes les créatures (1-5)
    }

    switch (type_creature) {
        case 1: return create_kraken();
        case 2: return create_requin();
        case 3: return create_meduse();
        case 4: return create_poisson_epee();
        case 5: return create_crabe_geant();
        default: return create_meduse();
    }
}

// Générer 1-4 créatures selon la profondeur
int generer_groupe_creatures(CreatureMarine creatures[], int max_creatures, int profondeur) {
    int nb_creatures;
    int prof_abs = abs(profondeur);

    // Plus on descend, plus il y a de créatures
    if (prof_abs == 0) {
        nb_creatures = 1; // Surface: 1 créature
    } else if (prof_abs <= 500) {
        nb_creatures = rand() % 3 + 1; // Zone facile: 1-3 créatures
    } else if (prof_abs <= 1500) {
        nb_creatures = rand() % 4 + 2; // Zone moyenne: 2-5 créatures
    } else {
        nb_creatures = rand() % 5 + 2; // Zone difficile: 2-6 créatures
    }

    if (nb_creatures > max_creatures) {
        nb_creatures = max_creatures;
    }

    for (int i = 0; i < nb_creatures; i++) {
        creatures[i] = generer_creature_aleatoire(profondeur);
        creatures[i].id = i; // ID unique pour cibler
    }

    return nb_creatures;
}

// Créer une créature par son ID (1-6)
// IDs: 1=Kraken, 2=Requin, 3=Méduse, 4=Poisson-Épée, 5=Crabe, 6=Boss
CreatureMarine creerCreature(int id, int profondeur __attribute__((unused))) {
    switch (id) {
        case 1: return create_kraken();
        case 2: return create_requin();
        case 3: return create_meduse();
        case 4: return create_poisson_epee();
        case 5: return create_crabe_geant();
        case 6: return create_kraken();  // Boss = Kraken amélioré (stats modifiées dans map.c)
        default: return create_meduse(); // Par défaut
    }
}

// Afficher une créature
void print_creature(CreatureMarine creature) {
    printf("--- %s ---\n", creature.nom);
    printf("PV: %d / %d\n", creature.points_de_vie_actuels, creature.points_de_vie_max);
    printf("Attaque: %d-%d\n", creature.attaque_minimale, creature.attaque_maximale);
    printf("Defense: %d\n", creature.defense);
    printf("Vitesse: %d\n", creature.vitesse);
    printf("Effet: %s\n", creature.effet_special);
    printf("Statut: %s\n", creature.est_vivant ? "Vivant" : "Mort");
    printf("--------------------------\n");
}
