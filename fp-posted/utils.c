#include "defs.h"

/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}

/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;
        
    }
}

void gethuntertheEvidence(RoomType* Room, HunterType* hunter){
    // Check if the room is not NULL
    if (Room->nextEvidence != NULL) {
        // Get the head of the evidence list
        EvidenceNodeType* currentEvidenceNode = Room->nextEvidence->head;

        // Traverse the evidence list
        while (currentEvidenceNode != NULL) {
            // Access the evidence data
            enum EvidenceType currentEvidence = currentEvidenceNode->data;
            char currentEvidencestr[MAX_STR];
            evidenceToString(currentEvidence,currentEvidencestr);
            char hunterevidencestr[MAX_STR];
            evidenceToString(hunter->evidence,currentEvidencestr);
            if (strcmp(currentEvidencestr,hunterevidencestr)==0)
            {
                // Create a new node for the evidence
                EvidenceNodeType* newEvidenceNode = malloc(sizeof(EvidenceNodeType));
                if (newEvidenceNode != NULL) {
                    newEvidenceNode->data = currentEvidenceNode->data;
                    newEvidenceNode->nextEvidence = NULL;

                    // Add the new node to the end of the hunter's evidence list
                    if (hunter->evidences == NULL) {
                        // The list is empty, so this is the first node
                        hunter->evidences->head = newEvidenceNode;
                        break;
                    } else {
                        // Find the last node in the list
                        EvidenceNodeType* lastNode = hunter->evidences->tail;
                        while (lastNode->nextEvidence != NULL) {
                            lastNode = lastNode->nextEvidence;
                        }
                        // Add the new node to the end of the list
                        lastNode->nextEvidence = newEvidenceNode;
                        break;
                    }

                } else {
                    printf("Error! Could not allocate memory for new evidence node.\n");
                }
            }

            // Move to the next evidence node
            currentEvidenceNode = currentEvidenceNode->nextEvidence;
        }
        l_hunterCollect(hunter->name, hunter->evidence, hunter->rooms->name);

        removeTheEvidenceFromRoom(Room, hunter->evidence);

    } else {
        printf("Error! Room is Evidence\n");
    }
}

void removeTheEvidenceFromRoom(RoomType* room, enum EvidenceType evidence){
    // Check if the room is not NULL
    if (room->nextEvidence != NULL) {
        // Get the head of the evidence list
        EvidenceNodeType* currentEvidenceNode = room->nextEvidence->head;
        EvidenceNodeType* previousEvidenceNode = NULL;

        // Traverse the evidence list
        while (currentEvidenceNode != NULL) {
            // Access the evidence data
            enum EvidenceType currentEvidence = currentEvidenceNode->data;
            char currentEvidencestr[MAX_STR];
            evidenceToString(currentEvidence,currentEvidencestr);
            char evi[MAX_STR];
            evidenceToString(evidence,evi);
            if (strcmp(currentEvidencestr,evi)==0)
            {
                // If the node to be deleted is the head node
                if (currentEvidenceNode == room->nextEvidence->head) {
                    room->nextEvidence->head = currentEvidenceNode->nextEvidence;
                } else {
                    // Else, we bypass the node to be deleted
                    previousEvidenceNode->nextEvidence = currentEvidenceNode->nextEvidence;
                }
                free(currentEvidenceNode);
                break;
            }

            // Move to the next evidence node
            previousEvidenceNode = currentEvidenceNode;
            currentEvidenceNode = currentEvidenceNode->nextEvidence;
        }
    } else {
        printf("Error! Room is Evidence\n");
    }
}

int checkerToExit(EvidenceListType*sharedList){
    // Get the head of the evidence list
    EvidenceNodeType* currentEvidenceNode = sharedList->head;
    EvidenceNodeType* currentEvidenceNode2 = sharedList->head;
    int counter = 0;
    EvidenceType validEvidence[validEvi];

    // Traverse the evidence list
    while (currentEvidenceNode != NULL || currentEvidenceNode2 != NULL) {
        // Access the evidence data
        EvidenceType currentEvidence = currentEvidenceNode->data;
        currentEvidenceNode2 = currentEvidenceNode2->nextEvidence;
        EvidenceType currentEvidence2 = currentEvidenceNode2->data;
        char evidence[MAX_STR];
        char evidence2[MAX_STR];

        evidenceToString(currentEvidence, evidence);
        evidenceToString(currentEvidence2, evidence2);
        if (strcmp(evidence,evidence2)!=0)
        {
            if (counter == 0)
            {
                validEvidence[counter] = counter;
                
                counter++;
                validEvidence[counter] = counter;

            }else{
               
                
           
                for(int i = 0; i<counter;i++){
                    char vEvidence[MAX_STR];
                    evidenceToString(validEvidence[i],vEvidence);
                    if(strcmp(vEvidence,evidence)==0){
                        break;
                    }else{
                        validEvidence[counter] = counter;
                    }
                }
            
            }
            counter++;

        }
        


        
        
        currentEvidenceNode = currentEvidenceNode->nextEvidence;
    }
    if(sizeof(validEvidence)==3){
        return 3;
    }else{
        return 4;
    }
}

