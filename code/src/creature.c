#include "creature.h"
#include "tab.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


CreatureMarine get_creature_by_id(int id) {
    for (int i = 0; i < g_creature_count; i++) {
        if (g_creature_database[i].id == id) {
            CreatureMarine copy = g_creature_database[i]; 
            
            copy.points_de_vie_max = randomNumber(copy.points_de_vie_max, copy.points_de_vie_max);
            copy.points_de_vie_actuels = copy.points_de_vie_max;
            
            return copy;
        }
    }
    return g_creature_database[0];
}

void print_creature(CreatureMarine creature) {
    printf("--- %s ---\n", creature.nom);
    printf("PV: %d / %d\n", creature.points_de_vie_actuels, creature.points_de_vie_max);
    printf("Attaque: %d-%d\n", creature.attaque_minimale, creature.attaque_maximale);
    printf("Défense: %d\n", creature.defense);
    printf("Vitesse: %d\n", creature.vitesse);
    printf("Statut: %s\n", creature.est_vivant ? "Vivant" : "Mort");
    printf("--------------------------\n");
}