#include "equipement.h"
#include "tab.h"
#include <string.h>
#include <stdlib.h>

Equipement get_equipement_by_id(int id) {
    for (int i = 0; i < g_equipement_count; i++) {
        if (g_equipement_database[i].id == id) {
            return g_equipement_database[i];
        }
    }
    // Si non trouvé, renvoie le premier équipement (Neoprene Basic)
    return g_equipement_database[0];
}


int get_defense_equipement(Equipement* equipement) {
    if (!equipement) return 0;
    // Note: vos configs ont min=max, donc la moyenne est la même.
    return (equipement->defense_minimale + equipement->defense_maximale) / 2;
}

int get_reduction_oxygene(Equipement* equipement) {
    if (!equipement) return 0;
    return equipement->consommation_oxygene_reduction;
}