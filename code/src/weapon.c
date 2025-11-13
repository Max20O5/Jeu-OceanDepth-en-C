#include "weapon.h"
#include <string.h>

// Trident rouillé: Arme de départ faible (10-15)
Arme creer_trident_rouille(void) {
    Arme trident;
    trident.id = 1;
    strcpy(trident.nom, "Trident Rouille");
    trident.attaque_minimale = 10;
    trident.attaque_maximale = 15;
    trident.consommation_oxygene = 2;
    trident.special = 0;
    trident.nb_utilisations = 0;
    trident.nb_utilisations_max = 0;
    trident.nb_utilisations_special = 0;
    trident.nb_utilisations_special_max = 0;
    trident.vitesse_minimale = 0;
    trident.vitesse_maximale = 0;
    strcpy(trident.effet_special, "aucun");
    trident.zone = 1;  // Disponible dès la zone 1
    return trident;
}

// Harpon Rouillé: Arme de base (12-18)
Arme creer_harpon_rouille(void) {
    Arme harpon;
    harpon.id = 2;
    strcpy(harpon.nom, "Harpon Rouille");
    harpon.attaque_minimale = 12;
    harpon.attaque_maximale = 18;
    harpon.consommation_oxygene = 2;
    harpon.special = 0;
    harpon.nb_utilisations = 0;
    harpon.nb_utilisations_max = 0;
    harpon.nb_utilisations_special = 0;
    harpon.nb_utilisations_special_max = 0;
    harpon.vitesse_minimale = 0;
    harpon.vitesse_maximale = 0;
    strcpy(harpon.effet_special, "aucun");
    harpon.zone = 1;  // Disponible dès la zone 1
    return harpon;
}

// Harpon d'Acier: Arme intermédiaire (20-28)
Arme creer_harpon_acier(void) {
    Arme harpon;
    harpon.id = 3;
    strcpy(harpon.nom, "Harpon d'Acier");
    harpon.attaque_minimale = 20;
    harpon.attaque_maximale = 28;
    harpon.consommation_oxygene = 3;
    harpon.special = 0;
    harpon.nb_utilisations = 0;
    harpon.nb_utilisations_max = 0;
    harpon.nb_utilisations_special = 0;
    harpon.nb_utilisations_special_max = 0;
    harpon.vitesse_minimale = 0;
    harpon.vitesse_maximale = 0;
    strcpy(harpon.effet_special, "aucun");
    harpon.zone = 2;  // Disponible à partir de la zone 2
    return harpon;
}

// Harpon Électrique: Arme rare (28-42) avec effet électrique
Arme creer_harpon_electrique(void) {
    Arme harpon;
    harpon.id = 4;
    strcpy(harpon.nom, "Harpon Electrique");
    harpon.attaque_minimale = 28;
    harpon.attaque_maximale = 42;
    harpon.consommation_oxygene = 4;
    harpon.special = 3; // Ignore 3 points de défense
    harpon.nb_utilisations = 0;
    harpon.nb_utilisations_max = 0;
    harpon.nb_utilisations_special = 0;
    harpon.nb_utilisations_special_max = 0;
    harpon.vitesse_minimale = 0;
    harpon.vitesse_maximale = 0;
    strcpy(harpon.effet_special, "electrique");
    harpon.zone = 3;  // Disponible à partir de la zone 3
    return harpon;
}

// Harpon Laser: Arme légendaire (35-50)
Arme creer_harpon_laser(void) {
    Arme harpon;
    harpon.id = 5;
    strcpy(harpon.nom, "Harpon Laser");
    harpon.attaque_minimale = 35;
    harpon.attaque_maximale = 50;
    harpon.consommation_oxygene = 5;
    harpon.special = 5; // Ignore 5 points de défense
    harpon.nb_utilisations = 0;
    harpon.nb_utilisations_max = 0;
    harpon.nb_utilisations_special = 0;
    harpon.nb_utilisations_special_max = 0;
    harpon.vitesse_minimale = 0;
    harpon.vitesse_maximale = 0;
    strcpy(harpon.effet_special, "laser");
    harpon.zone = 4;  // Disponible à partir de la zone 4
    return harpon;
}