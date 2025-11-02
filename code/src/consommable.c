#include "consommable.h"
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

Consommable creer_trousse_de_soin(void) {
    Consommable soin;
    soin.id = 1;
    strcpy(soin.nom, "Trousse de Soin");
    strcpy(soin.effet_special, "soin");
    soin.soin = 25;
    soin.oxygene = 0;
    soin.fatigue = 0;
    return soin;
}

Consommable creer_capsule_oxygene(void) {
    Consommable oxygene;
    oxygene.id = 2;
    strcpy(oxygene.nom, "Capsule O2");
    strcpy(oxygene.effet_special, "oxygene");
    oxygene.soin = 0;
    oxygene.oxygene = 40;
    oxygene.fatigue = 0;
    return oxygene;
}