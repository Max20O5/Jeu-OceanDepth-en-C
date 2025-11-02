#include "weapon.h"
#include <string.h>

Arme creer_harpon_rouille(void) {
    Arme harpon;

    harpon.id = 1;
    strcpy(harpon.nom, "Harpon Rouillé");
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

    return harpon;
}