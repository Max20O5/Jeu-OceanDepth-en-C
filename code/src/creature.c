#include "creature.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

// Création du Kraken
CreatureMarine create_kraken() {
    CreatureMarine kraken;

    kraken.id = 1;
    strcpy(kraken.nom, "Kraken");
    kraken.points_de_vie_max = randomNumber(150, 200);
    kraken.points_de_vie_actuels = kraken.points_de_vie_max;
    kraken.attaque_minimale = randomNumber(20, 30);
    kraken.attaque_maximale = randomNumber(30, 50);
    kraken.defense = randomNumber(10, 20);
    kraken.vitesse = randomNumber(5, 15);
    strcpy(kraken.effet_special, "Tentacules enchevetrantes de le mort qui tue");
    kraken.est_vivant = true;
    
    return kraken;
}

/** 
CreatureMarine create_crabe() {
    CreatureMarine crabe;
...
    return crabe;
}
**/


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