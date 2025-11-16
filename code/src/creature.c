#include "creature.h"
#include "tab.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Création du Kraken
CreatureMarine create_kraken() {
    CreatureMarine kraken;

    kraken.id = 1;
    strcpy(kraken.nom, "Kraken");
    kraken.points_de_vie_max = randomNumber(10, 30);
    kraken.points_de_vie_actuels = kraken.points_de_vie_max;
    kraken.attaque_minimale = randomNumber(20, 30);
    kraken.attaque_maximale = randomNumber(30, 50);
    kraken.defense = randomNumber(10, 20);
    kraken.vitesse = randomNumber(5, 15);
    kraken.perles_min = 25;
    kraken.perles_max = 60;
    strcpy(kraken.effet_special, "Paralysie");
    kraken.est_vivant = true;
    
    return kraken;
}

CreatureMarine get_creature_by_id(int id) {
    for (int i = 0; i < g_creature_count; i++) {
        if (g_creature_database[i].id == id) {
            CreatureMarine copy = g_creature_database[i];

            // Randomiser les PV entre min et max
            copy.points_de_vie_max = randomNumber(copy.points_de_vie_min, copy.points_de_vie_max);
            copy.points_de_vie_actuels = copy.points_de_vie_max;
            copy.est_vivant = true;
            copy.active_effect_count = 0;

            return copy;
        }
    }
    // Si non trouvé, retourner une créature vide
    CreatureMarine empty;
    memset(&empty, 0, sizeof(CreatureMarine));
    strcpy(empty.nom, "ERREUR");
    return empty;
}
// Afficher le mob
void print_creature(CreatureMarine creature) {
    printf("--- %s ---\n", creature.nom);
    printf("PV: %d / %d\n", creature.points_de_vie_actuels, creature.points_de_vie_max);
    printf("Attaque: %d-%d\n", creature.attaque_minimale, creature.attaque_maximale);
    printf("Défense: %d\n", creature.defense);
    printf("Vitesse: %d\n", creature.vitesse);
    printf("Statut: %s\n", creature.est_vivant ? "Vivant" : "Mort");
    printf("--------------------------\n");
}