#include "weapon.h"
#include "tab.h"
#include <string.h>

Arme get_weapon_by_id(int id) {
    for (int i = 0; i < g_weapon_count; i++) {
        if (g_weapon_database[i].id == id) {
            return g_weapon_database[i];
        }
    }
    return g_weapon_database[0]; 
}