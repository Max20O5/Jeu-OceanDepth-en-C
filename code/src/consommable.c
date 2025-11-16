#include "consommable.h"
#include "tab.h"
#include <string.h>

Consommable get_consommable_by_id(int id) {
    // Parcourir la base de données
    for (int i = 0; i < g_consommable_count; i++) {
        if (g_consommable_database[i].id == id) {
            return g_consommable_database[i];
        }
    }
    
    return g_consommable_database[0]; 
}
