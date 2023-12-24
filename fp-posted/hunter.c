#include "defs.h"

HunterType* initHunters(char*hunterName){
   HunterType* hunter = (HunterType*)malloc(sizeof(HunterType));
   strcpy(hunter->name, hunterName);
   hunter->boredomTimer = 0;
   hunter->fearLevel = 0;
   //Add enum and evidence node if necessary
   return hunter;
}

void addHuntersToList(HunterType* hunter, HunterListType* hunterList){
    // Check for duplicates
    HunterNodeType* current = hunterList->head;
    while (current != NULL) {
        if (current->hunter == hunter) {
            printf("Duplicate hunter found. Not adding to list.\n");
            return;
        }
        current = current->next;
    }

    // If no duplicate found, add the new hunter to the list
    HunterNodeType *hunterHolder = (HunterNodeType*)malloc(sizeof(HunterNodeType));
    if (hunterHolder  == NULL){
        printf("Memory allocation failed. \n");
        exit(1);
    }

    hunterHolder->hunter = hunter;
    hunterHolder->next = NULL;

    if(hunterList->head == NULL){
        hunterList->head = hunterHolder;	  
    } else{
        hunterList->tail->next = hunterHolder;
    }

    hunterList->tail = hunterHolder;
}





