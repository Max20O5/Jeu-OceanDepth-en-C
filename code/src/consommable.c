#include "consommable.h"
#include "tab.h"
#include <string.h>

Consommable creer_slot_vide(void) {
    Consommable vide;
    vide.id = 0;
    strcpy(vide.nom, "Vide");
    strcpy(vide.effet_special, "aucun");
    vide.soin = 0;
    vide.oxygene = 0;
    vide.fatigue = 0;
    return vide;
}

Consommable get_consommable_by_id(int id) {
    // Parcourir la base de données
    for (int i = 0; i < g_consommable_count; i++) {
        if (g_consommable_database[i].id == id) {
            return g_consommable_database[i];
        }
    }
    
    return g_consommable_database[0]; 
}
