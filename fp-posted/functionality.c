#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global exit flag
int exitFlag = 0;
pthread_mutex_t exitFlagLock;

void* ghostBehaviour(void* arg){
    CombinationType* combinationStruct = (CombinationType*) arg;
    GhostType* ghost = combinationStruct->ghost;

    while (!exitFlag && ghost->timer < BOREDOM_MAX) {
        usleep(GHOST_WAIT*2);
        pthread_mutex_lock(&(combinationStruct->lock));
        HunterNodeType* currentNode = combinationStruct->hunterList->head;
        if(currentNode != NULL && !exitFlag) {
            if(strcmp(currentNode->hunter->rooms->name, ghost->room->name) == 0){
                // If the Ghost is in the room with a hunter, reset the Ghost’s boredom timer to 0
                ghost->timer = 0;
                printf("Ghost is in the room with a hunter. Boredom timer reset to 0.\n");

                // Randomly choose to leave evidence or to do nothing
                int action = randInt(0, 2);
                if (action == 0) {
                    // Leave evidence
                    addEvidenceToRoom(combinationStruct, ghost->room);
                    // Implement the logic for leaving evidence here
                } 
            } else {
                // If the Ghost is not in the room with a hunter, increase the Ghost’s boredom counter by 1
                ghost->timer++;
              
                // Randomly choose to move to an adjacent room, to leave evidence, or to do nothing
                int action = randInt(0, 3);
                if (action == 0) {
                    // Move to an adjacent room
                    
                    RoomType*randomRoom  = roomRandomizer(ghost->room);
                    randomRoom->ghost = ghost;
                    ghost->room = randomRoom;
                    l_ghostMove(ghost->room->name);

                    // Implement the logic for moving to an adjacent room here
                } else if (action == 1) {
                    // Leave evidence
                    addEvidenceToRoom(combinationStruct, ghost->room);
                } 
            }

            currentNode = currentNode->next;
        }


        // Check the global exit flag
        pthread_mutex_lock(&exitFlagLock);
        if (exitFlag || ghost->timer == BOREDOM_MAX) {
              printf("%d",exitFlag);
            pthread_mutex_unlock(&exitFlagLock);
            exitFlag = 1;
            l_ghostExit(exitFlag);
            pthread_mutex_unlock(&(combinationStruct->lock));
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&exitFlagLock);
        pthread_mutex_unlock(&(combinationStruct->lock));
    }
    return NULL;
}

void* hunterBehaviour(void* arg){
    CombinationType* combinationStruct = (CombinationType*) arg;
    HunterType* hunter = combinationStruct->hunter;
    GhostType* ghost = combinationStruct->ghost;
    while (!exitFlag && hunter->boredomTimer < BOREDOM_MAX && hunter->fearLevel < FEAR_MAX) {
        usleep(HUNTER_WAIT);
        pthread_mutex_lock(&(combinationStruct->lock));
        HunterNodeType* currentNode = combinationStruct->hunterList->head;
        if(currentNode != NULL) {
            if(strcmp(currentNode->hunter->rooms->name, ghost->room->name) == 0){
                hunter->fearLevel++;
                hunter->boredomTimer = 0;
                
            } else {
                hunter->boredomTimer++;
            }
            int action = randInt(0, 3);
            if (action == 0) {
                    printf("Hunter is reviewing evidence.\n");
                    exitFlag = checkerToExit(hunter->evidences);
                    l_hunterReview(hunter->name, exitFlag);
                    if (exitFlag == 3) {
                        l_hunterExit(hunter->name, 3);
                        pthread_exit(NULL);
                    }else{
                        exitFlag = 0;
                    }


            } else if(action == 1){
                    RoomType* randomRoom  = roomRandomizer(hunter->rooms);
                    moveHunterToRandomRoom(randomRoom, hunter);
                    addRandomRoomToHunter(randomRoom, hunter);
                    l_hunterMove(hunter->name, randomRoom->name);
            }else {
                    gethuntertheEvidence(hunter->rooms, hunter);
            }
        }
        if (hunter->boredomTimer == BOREDOM_MAX)
        {
                l_hunterExit(hunter->name, 1);
        }else if (hunter->fearLevel == FEAR_MAX){
                l_hunterExit(hunter->name, 0);
        }
        if (exitFlag || hunter->boredomTimer == BOREDOM_MAX || hunter->fearLevel == FEAR_MAX) {
            printf("exitFlag = %d, bored = %d, fearMax = %d", exitFlag,hunter->boredomTimer,hunter->fearLevel);
            exitFlag = 1;
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&(combinationStruct->lock));
        
    }
    return NULL;
}
