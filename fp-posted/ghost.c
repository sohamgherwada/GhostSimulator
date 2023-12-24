#include "defs.h"
char ghostString[16];
int initGhost(enum GhostClass givenGhost, RoomType* givenRoom,GhostType* ghost){
    int randomValue = randInt(C_FALSE,10);
    if(randomValue>=7){

        ghost->gc = givenGhost;
        ghost->room = givenRoom;
        ghost->timer = 0;
    } 
    return randomValue;
}
void assignRandomEvidence(CombinationType * forEvidedence){
    for (int i = 0; i < validEvi; i++) {
        forEvidedence->ve[i] = randInt(0,validEvi+1);  // Assign a random number between 0 and 3
        
    }
}



