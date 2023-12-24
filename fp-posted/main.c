
#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));
    pthread_mutex_t exitpro_lock;

    // Initialize the mutex lock for exitpro
    pthread_mutex_init(&exitpro_lock, NULL);

    char hunterNameOne[MAX_STR];
    char hunterNameTwo[MAX_STR];
    char hunterNameThree[MAX_STR];
    char hunterNameFour[MAX_STR];

    
    
   
   

    
    printf("Input a name for hunter 1?");
    scanf("%s",hunterNameOne);
    HunterType* hunter1 = initHunters(hunterNameOne);
    hunter1->evidence = EMF;
    l_hunterInit(hunter1->name, hunter1->evidence); 

    printf("Input a name for hunter 2");
    scanf("%s", hunterNameTwo);
    HunterType* hunter2 = initHunters(hunterNameTwo);
    hunter2->evidence = TEMPERATURE;
    l_hunterInit(hunter2->name, hunter2->evidence);

    
    printf("Input a name for hunter 3");
    scanf("%s", hunterNameThree); 
    HunterType* hunter3 = initHunters(hunterNameThree);
    hunter3->evidence = FINGERPRINTS;
    l_hunterInit(hunter3->name, hunter3->evidence);

    printf("Input a name for hunter 4");
    scanf("%s", hunterNameFour); 
    HunterType* hunter4 = initHunters(hunterNameFour);
    hunter4->evidence = SOUND;

    l_hunterInit(hunter4->name, hunter4->evidence);

    


    HouseType* house = (HouseType*)malloc(sizeof(HouseType));
    house->rooms = (RoomListType*)malloc(sizeof(RoomListType));
    house->hunters = (HunterListType*)malloc(sizeof(HunterListType));
    house->evidences = (EvidenceListType*)malloc(sizeof(EvidenceListType));

    hunter1->evidences = house->evidences;
    hunter2->evidences = house->evidences;
    hunter3->evidences = house->evidences;
    hunter4->evidences = house->evidences;
   
    
    


    populateRooms(house);

    addHuntersToList(hunter1,house->hunters);
    addHuntersToList(hunter2,house->hunters);
    addHuntersToList(hunter3,house->hunters);
    addHuntersToList(hunter4,house->hunters);

    hunter1->rooms = house->rooms->head->room;
    hunter2->rooms = house->rooms->head->room;
    hunter3->rooms = house->rooms->head->room;
    hunter4->rooms = house->rooms->head->room;

    house->rooms->head->room->hunters = house->hunters;

    GhostType* ghost = (GhostType*)malloc(sizeof(GhostType));
    
    if(house->rooms->head == NULL){
        printf("The hunters list is empty.\n");
    } else {
        NodeType* currentNode = house->rooms->head->next;
        int value = 0;
        while(currentNode != NULL) {
            int randEnum = randInt(0,5);
            value = initGhost(randEnum, currentNode->room,ghost);
            
            if (value>=7) {
                l_ghostInit(ghost->gc,ghost->room->name);
                break;
            }
            currentNode = currentNode->next;
        }
    }
    pthread_t hunterThread1, hunterThread2, hunterThread3, hunterThread4, ghostThread;

    CombinationType* combinationStruct = (CombinationType* ) malloc(sizeof(CombinationType));
    combinationStruct->ghost = ghost;
    assignRandomEvidence(combinationStruct);
    combinationStruct->hunterList = house->hunters;
    combinationStruct->rooms = house->rooms;

    CombinationType* combinationStruct1 = (CombinationType*) malloc(sizeof(CombinationType));
    combinationStruct1->ghost = ghost;
    combinationStruct1->hunter = hunter1;
    combinationStruct1->hunterList = house->hunters;
    combinationStruct1->rooms = house->rooms;
    pthread_mutex_init(&(combinationStruct1->lock), NULL);

    CombinationType* combinationStruct2 = (CombinationType*) malloc(sizeof(CombinationType));
    *combinationStruct2 = *combinationStruct1; // Copy the contents of combinationStruct1 to combinationStruct2
    combinationStruct2->hunter = hunter2;

    CombinationType* combinationStruct3 = (CombinationType*) malloc(sizeof(CombinationType));
    *combinationStruct3 = *combinationStruct1; // Copy the contents of combinationStruct1 to combinationStruct3
    combinationStruct3->hunter = hunter3;

    CombinationType* combinationStruct4 = (CombinationType*) malloc(sizeof(CombinationType));
    *combinationStruct4 = *combinationStruct1; // Copy the contents of combinationStruct1 to combinationStruct4
    combinationStruct4->hunter = hunter4;

    // Create threads for the hunters
    pthread_create(&hunterThread1, NULL, hunterBehaviour, combinationStruct1);
    pthread_create(&hunterThread2, NULL, hunterBehaviour, combinationStruct2);
    pthread_create(&hunterThread3, NULL, hunterBehaviour, combinationStruct3);
    pthread_create(&hunterThread4, NULL, hunterBehaviour, combinationStruct4);

    pthread_mutex_init(&(combinationStruct->lock), NULL);


    pthread_create(&ghostThread, NULL, ghostBehaviour, combinationStruct);
    
    pthread_join(ghostThread, NULL);
    
    pthread_join(hunterThread1, NULL);
    pthread_join(hunterThread2, NULL);
    pthread_join(hunterThread3, NULL);
    pthread_join(hunterThread4, NULL);

    // Destroy the mutex lock before exiting
    pthread_mutex_destroy(&exitpro_lock);
    printf("The program has ended");
    //add clean up functions
    
    cleanupHouse(house);
    return 0;
}
