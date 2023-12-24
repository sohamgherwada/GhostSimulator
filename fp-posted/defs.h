#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define validEvi        3
#define FEAR_MAX        10
#define LOGGING         C_TRUE


typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef struct House HouseType;
typedef struct Room RoomType;
typedef struct Ghost GhostType;
typedef struct Hunter HunterType;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct HunterNode HunterNodeType;

typedef struct RoomNextlist RoomNextlistType;
typedef struct RoomList RoomListType;
typedef struct Node NodeType;
typedef struct HunterList HunterListType;
typedef struct Combination CombinationType;
typedef struct EvidenceList EvidenceListType;



enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

struct Room 
{
   char name[MAX_STR];
   RoomListType* nextRoom;
   EvidenceListType*nextEvidence;
   HunterListType* hunters;
   GhostType* ghost;
};

struct Node
{
   struct Room *room;
   struct Node *next;
};

struct RoomList
{
  struct Node* head;
  struct Node* tail;
  RoomListType* next;
};




struct EvidenceNode
{
    enum EvidenceType data;
    EvidenceNodeType* nextEvidence;
};

struct EvidenceList
{
   EvidenceNodeType*head;
   EvidenceNodeType*tail;
   EvidenceListType*next;
};


struct Ghost
{
    GhostClass gc;
    EvidenceType validEvidence[validEvi];
    RoomType* room; //We need to get the exact room the hunter is in 
    int timer;//Boredom Timer   
};

struct Hunter
{
    RoomType* rooms; //We need to get the exact room the hunter is in 
    char name[MAX_STR];
    enum EvidenceType evidence;
    int boredomTimer;
    int fearLevel;
    EvidenceListType* evidences;

    
};
struct HunterList
{
  HunterNodeType* head;
  HunterNodeType* tail;
  HunterListType* next;
};

struct HunterNode
{
    struct Hunter *hunter;
    HunterNodeType* next;

};

struct House
{
    RoomListType* rooms;
    HunterListType* hunters;
    EvidenceListType* evidences;
    
    
};

struct Combination
{
    GhostType* ghost;
    RoomListType* rooms;
    HunterListType* hunterList;
    HunterType* hunter;
    enum EvidenceType ve[validEvi];
    pthread_mutex_t lock;
};

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter


// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);



//This init's for all the structs
HunterType* initHunters(char*hunterName);
int initGhost(enum GhostClass givenGhost, RoomType* givenRoom,GhostType* ghost);
//
RoomType* createRoom(char*roomname);
void addRoom(RoomListType*roomList, RoomType*room);
void connectRooms(RoomType*room1,RoomType*room2);
void populateRooms(HouseType* house);
void addHuntersToList(HunterType* hunter, HunterListType* hunterList);
void* ghostBehaviour(void* arg);
void* hunterBehaviour(void* arg);
RoomType*roomRandomizer(RoomType*room);
RoomType*moveGhostToRandomRoom(RoomType*room, GhostType*ghost, RoomListType*roomList);
void addRandomRoomToGhost(RoomType*room, GhostType*ghost);
void addEvidenceToRoom(CombinationType*forEvidence, RoomType* room);

RoomType* moveHunterToRandomRoom(RoomType*room, HunterType*hunter);
void addRandomRoomToHunter(RoomType*room, HunterType*hunter);
void gethuntertheEvidence(RoomType*Room,HunterType*hunter);
void removeTheEvidenceFromRoom(RoomType* room, enum EvidenceType evidence);
int checkerToExit(EvidenceListType*sharedList);
void assignRandomEvidence(CombinationType * forEvidedence);


void cleanupHunterNode(HunterListType* hunterList);
void cleanupHunterList(HunterListType* hunterList);
void cleanupEvidenceNode(EvidenceListType* evidenceList);
void cleanupEvidenceList(EvidenceListType* evidenceList);
void cleanupRoom(RoomType* room);
void cleanupRoomNode(RoomListType* roomList) ;
void cleanupRoomList(RoomListType* roomList);

void cleanupHouse(HouseType* house);
void cleanupGhost(GhostType* ghost);