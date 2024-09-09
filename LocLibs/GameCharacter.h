///Holds universal character data
#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

//I need these macros all over bank 1 and all over bank 1 happens to include this header. Bad practice but it saves me space
#define SR2 SWITCH_ROM(2)
#define SR1 SWITCH_ROM(1)
#define SR0 SWITCH_ROM(0)

#include "gb/gb.h" // Include necessary headers

//Fits all characters. Amount of sprites, position, graphics, points when caught
typedef struct GameCharacter{
    UBYTE spritids[4];
    UBYTE palids[4];
    uint8_t xLoc; 
    uint8_t yLoc; 
    uint8_t width;
    uint8_t height;
    int8_t Xdir;
    int8_t Ydir;
    uint8_t timer; //If a fish needs to stop for a set amount of time (mostly for zig-zag behavior)
    uint8_t scoreValue;
    uint8_t respawnTimer; //different than regular timer because it created conflicts
    uint8_t animDelayer;
    uint8_t currentSkin; //used for death animations
    uint8_t myScoreMult; //gives score mult at time of death so it can be displayed during death anim
    UBYTE selectMovement;
    UBYTE flippedForDeath; //Boolean that checks that a fish has been flipped to face the correct way when they die. Helps calculate expensive functions less and thus reduce lag significantly
}GameCharacter;


typedef struct FishProfile{
//Setting every type of fish on startup so fish can respawn into new fish when they're caught
    UBYTE sprID[4];
    UBYTE palID[4];
    uint8_t speed;
    uint8_t scoreVal;
}FishProfile;


//I need the structs for this anyways. Why not put the finished product here?
GameCharacter man;
GameCharacter fish1;
GameCharacter fish2;
GameCharacter fish3;
GameCharacter fish4;
FishProfile snout;
FishProfile pink;
FishProfile puffer;
FishProfile clown;
FishProfile Angel;

GameCharacter *fishList[4] = {&fish1, &fish2, &fish3, &fish4}; //so I can iterate over all fish in 3 lines
FishProfile *skinList[5] = {&snout, &pink, &puffer, &clown, &Angel};

#endif /* GAME_CHARACTER_H */