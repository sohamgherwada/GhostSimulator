#include "defs.h"
/*
    This is for the room 
*/  
RoomType* createRoom(char*roomname){
    RoomType* newRoom = (RoomType*)malloc(sizeof(RoomType));
    strcpy(newRoom->name,roomname);
    newRoom->nextEvidence = (EvidenceListType*)malloc(sizeof(EvidenceListType));
    newRoom->hunters = (HunterListType*)malloc(sizeof(HunterListType));
    return newRoom;
}

void addRoom(RoomListType*roomList, RoomType*room){
       NodeType *roomHolder = (NodeType*)malloc(sizeof(NodeType));
       
       if (roomHolder  == NULL){
          printf("Memory allocation failed. \n");
          exit(1);
        }

       
       roomHolder->room = room;
       roomHolder->next = NULL;

       //If starting of list is null then set it as the newNode
       if(roomList->head == NULL){
          roomList->head = roomHolder;	  
       } else{
         //otherwise traverse through till the current last node and make it point to the new node
	     roomList->tail->next = roomHolder;
       }
       
       //update the tail
       roomList->tail = roomHolder;
}
   
void connectRooms(RoomType* room1, RoomType* room2) {
    if (room1 != NULL && room1->nextRoom == NULL) {
        room1->nextRoom = malloc(sizeof(RoomListType));
        room1->nextRoom->head = NULL;
        room1->nextRoom->tail = NULL;
    }

    if (room2 != NULL && room2->nextRoom == NULL) {
        room2->nextRoom = malloc(sizeof(RoomListType));
        room2->nextRoom->head = NULL;
        room2->nextRoom->tail = NULL;
    }

    if (room1 != NULL && room1->nextRoom != NULL && room1->nextRoom->head != NULL) {
        room1->nextRoom->head->room = room2;
    } 

    if (room2 != NULL && room2->nextRoom != NULL && room2->nextRoom->head != NULL) {
        room2->nextRoom->head->room = room1;
    } 
}




RoomType* roomRandomizer(RoomType*room){
    if(room->nextRoom->head == NULL){
        printf("The list is empty.\n");
    } else{
        NodeType* node = room->nextRoom->head;
        while (node != NULL) {
            int random = randInt(0,10);
            if(random >=7){
                return node->room;
            }

            node = node->next;
        }
        
    }
     return room;
}


RoomType* moveGhostToRandomRoom(RoomType*room, GhostType*ghost, RoomListType*roomList){
    if(roomList->head == NULL){
        printf("The list is empty.\n");
    } else{
        NodeType* roomNode = roomList->head;
        do {
            if (strcmp(roomNode->room->name, room->name) == 0){
                roomNode->room->ghost = ghost;  
                return roomNode->room;                  
            }
            roomNode = roomNode->next;
        } while(roomNode != roomList->head);
    }
    return room;
}
void addRandomRoomToGhost(RoomType*room, GhostType*ghost){
    ghost->room = room;
}

RoomType* moveHunterToRandomRoom(RoomType*room, HunterType*hunter){
    
    if (room->hunters->head == NULL)
    {
        room->hunters->head= (HunterNodeType*)malloc(sizeof(HunterNodeType));
        room->hunters->head->hunter = hunter;
        room->hunters->head->next = NULL;
    }else
    {
        HunterNodeType* current = room->hunters->head;
        while(current != NULL)
        {
            if(current->hunter == hunter) // Assuming hunter comparison is done by reference
            {
                current->hunter = hunter; // Replace the hunter
                return room;
            }
            if(current->next == NULL) // We are at the end of the list
            {
                current->next = (HunterNodeType*)malloc(sizeof(HunterNodeType));
                current->next->hunter = hunter;
                current->next->next = NULL;

                return room;
            }
            current = current->next;
        }
    }
    
    return room;
}


void addRandomRoomToHunter(RoomType*room, HunterType*hunter){
        hunter->rooms = room;


}

void addEvidenceToRoom(CombinationType*forEvidence, RoomType* room){
    int random = randInt(0, validEvi);
    printf("\n I have a random evidence %d\n", random);

    // Create a new node
    EvidenceNodeType* newNode = malloc(sizeof(EvidenceNodeType));
    if (newNode == NULL) {
        printf("Failed to allocate memory for new node.\n");
        
        return;
    }

    
    newNode->data= forEvidence->ve[random];
    newNode->nextEvidence = NULL;

    // If the list is empty, make the new node the head
    if (room->nextEvidence->head == NULL) {
        room->nextEvidence->head = newNode;
    } else {
        // Otherwise, add the new node to the end of the list
        EvidenceNodeType* current = room->nextEvidence->head;
        while (current->nextEvidence != NULL) {
            current = current->nextEvidence;
        }
        current->nextEvidence = newNode;
    }
    l_ghostEvidence(forEvidence->ve[random], room->name);
}





