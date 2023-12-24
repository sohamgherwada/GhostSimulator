#include "defs.h"

// This function cleans up the data in the hunter nodes
void cleanupHunterNode(HunterListType* hunterList) {
    HunterNodeType* current = hunterList->head;
    while (current != NULL) {
        HunterNodeType* temp = current;
        current = current->next;
        free(temp->hunter); // Free the data in each individual hunter node
        temp->hunter = NULL;
    }
}

// This function cleans up the nodes in the hunter list
void cleanupHunterList(HunterListType* hunterList) {
    HunterNodeType* current = hunterList->head;
    HunterNodeType* next;
    while (current != NULL) {
        next = current->next;
        free(current); // Free each individual hunter node
        current = next;
    }
    hunterList->head = NULL;
    hunterList->tail = NULL;
}

// This function cleans up the data in the evidence nodes
void cleanupEvidenceNode(EvidenceListType* evidenceList) {
    EvidenceNodeType* current = evidenceList->head;
    EvidenceNodeType* next;

    while (current != NULL) {
        next = current->nextEvidence;
        free(current); 
        current = next;
    }
}


// This function cleans up the nodes in the evidence list
void cleanupEvidenceList(EvidenceListType* evidenceList) {
    EvidenceNodeType* current = evidenceList->head;
    EvidenceNodeType* next;
    while (current != NULL) {
        next = current->nextEvidence;
        free(current); // Free each individual evidence node
        current = next;
    }
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
}


// This function cleans up the data in the room nodes
void cleanupRoomNode(RoomListType* roomList) {
    NodeType* current = roomList->head;
    while (current != NULL) {
        NodeType* temp = current;
        current = current->next;
        free(temp->room); // Free the data in each individual room node
        temp->room = NULL;
    }
}

// This function cleans up the nodes in the room list
// This function cleans up the nodes in the room list
void cleanupRoomList(RoomListType* roomList) {
    NodeType* current = roomList->head;
    NodeType* next;
    while (current != NULL) {
        next = current->next;
        free(current); // Free each individual room node
        current = next;
    }
    roomList->head = NULL;
    roomList->tail = NULL;
}




void cleanupRoom(RoomType* room) {
    // Cleanup resources associated with the room structure, if any
    
    cleanupRoomNode(room->nextRoom);
    cleanupRoomList(room->nextRoom);
    cleanupEvidenceNode(room->nextEvidence);
    cleanupEvidenceList(room->nextEvidence);
    cleanupHunterNode(room->hunters);
    cleanupHunterList(room->hunters);
    room = NULL;
}

void cleanupGhost(GhostType* ghost) {
    // Cleanup resources associated with the ghost structure, if any
    // (Make sure to clean up any additional allocated resources here)
    free(ghost); // Deallocate the memory for the ghost structure
}

void cleanupHouse(HouseType* house) {
    // Clean up resources associated with the house structure
    cleanupRoomNode(house->rooms);
    cleanupRoomList(house->rooms);
    cleanupEvidenceNode(house->evidences);

    cleanupEvidenceList(house->evidences);
    cleanupHunterNode(house->hunters);

    cleanupHunterList(house->hunters);
}
