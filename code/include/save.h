#ifndef SAVE_H
#define SAVE_H

#include "struct.h"

// Carte is now defined in struct.h

#define SAVE_FILE "saves/oceandepth_save.txt"

// Sauvegarder la partie
bool sauvegarder_partie(Plongeur* joueur, Carte* carte);

// Charger la partie
bool charger_partie(Plongeur* joueur, Carte** carte);

// Vérifier si une sauvegarde existe
bool sauvegarde_existe();

// Supprimer la sauvegarde
void supprimer_sauvegarde();

#endif
