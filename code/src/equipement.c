#include "equipement.h"
#include <string.h>
#include <stdlib.h>

// Néoprène basique - Équipement de départ (+5 défense)
Equipement creer_neoprene_basic() {
    Equipement equip;
    equip.id = 1;
    strcpy(equip.nom, "Neoprene Basic");
    equip.defense_minimale = 5;
    equip.defense_maximale = 5;
    equip.vitesse_minimale = 0;
    equip.vitesse_maximale = 0;
    equip.consommation_oxygene_reduction = 0;
    strcpy(equip.effet_special, "aucun");
    return equip;
}

// Combinaison renforcée (+10 défense, -1 consommation oxygène)
Equipement creer_combinaison_renforcee() {
    Equipement equip;
    equip.id = 2;
    strcpy(equip.nom, "Combinaison Renforcee");
    equip.defense_minimale = 10;
    equip.defense_maximale = 10;
    equip.vitesse_minimale = 0;
    equip.vitesse_maximale = 0;
    equip.consommation_oxygene_reduction = 1;
    strcpy(equip.effet_special, "economie");
    return equip;
}

// Combinaison grande profondeur (+15 défense, -2 consommation oxygène)
Equipement creer_combinaison_profondeur() {
    Equipement equip;
    equip.id = 3;
    strcpy(equip.nom, "Combinaison Profondeur");
    equip.defense_minimale = 15;
    equip.defense_maximale = 15;
    equip.vitesse_minimale = 0;
    equip.vitesse_maximale = 0;
    equip.consommation_oxygene_reduction = 2;
    strcpy(equip.effet_special, "profondeur");
    return equip;
}

// Armure Titanium - Meilleur équipement (+25 défense, -3 consommation oxygène)
Equipement creer_armure_titanium() {
    Equipement equip;
    equip.id = 4;
    strcpy(equip.nom, "Armure Titanium");
    equip.defense_minimale = 25;
    equip.defense_maximale = 25;
    equip.vitesse_minimale = 0;
    equip.vitesse_maximale = 0;
    equip.consommation_oxygene_reduction = 3;
    strcpy(equip.effet_special, "superieur");
    return equip;
}

// Obtenir la défense d'un équipement (moyenne entre min et max)
int get_defense_equipement(Equipement* equipement) {
    if (!equipement) return 0;
    return (equipement->defense_minimale + equipement->defense_maximale) / 2;
}

// Obtenir la réduction de consommation d'oxygène
int get_reduction_oxygene(Equipement* equipement) {
    if (!equipement) return 0;
    return equipement->consommation_oxygene_reduction;
}
