#ifndef UNBANKEDFUNCTIONS_H
#define UNBANKEDFUNCTIONS_H

#include <gb/gb.h>
#include "../LocLibs/GameCharacter.h"

extern void snd_LevelUp(void);

extern void fishpicker(struct GameCharacter* fish, struct FishProfile* fishSkin);
extern uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt);
extern void objectflip(int8_t directionFromSpeed, GameCharacter* character, uint8_t x, uint8_t y);

extern const uint8_t numSprDict[10];



uint8_t levelup(uint8_t level){
    FishProfile *fishUpdaterArray[] = {&snout, &pink, &puffer, &clown, &Angel}; //Speed and score of all fish types

    level++;
    SWITCH_ROM(1);
    snd_LevelUp();

    if (level == 5){
        for (int i=0; i<5; i++){
            fishUpdaterArray[i]->scoreVal += 5;
            fishUpdaterArray[i]->speed += 2;
        }
    }
    if (level == 10){
        for (int i=0; i<5; i++){ 
            fishUpdaterArray[i]->scoreVal -= 5; //undoing previous level
            fishUpdaterArray[i]->speed -= 2;

            fishUpdaterArray[i]->scoreVal *= 2; //bringing the ruckus
            fishUpdaterArray[i]->speed *= 2;
        }
    }
    if (level == 15 || level == 30){ //Game still gets a bit harder over time. I've attempted to balance the pace a bit
        for (int i=0; i<5; i++){
            if (fishUpdaterArray[i]->scoreVal < 90){ //Ensures score value no longer increases past 99
                fishUpdaterArray[i]->scoreVal += 10;
            }
            fishUpdaterArray[i]->speed += 5;
        }
    }

    set_sprite_tile(25, numSprDict[level/10]);
    set_sprite_tile(26, numSprDict[level%10]);
    SWITCH_ROM(0);
    return level;
}


void fishDeathAnim(GameCharacter* fish, FishProfile* fishSkin){
    //SWITCH_ROM(1);
    if (fish->flippedForDeath == 0){ //Object flip is expensive so this makes sure it's only calculated once. Helps a lot with lag
        objectflip(-1, fish, fish->xLoc, fish->yLoc);

        //This block sets fish to stand completely still after they die. If any of them are missing then the fish does a lil dance
        fish->Xdir = 0;
        fish->Ydir = 0;
        fish->selectMovement = 1;
        fish->flippedForDeath = 1;
    }

    // numSprDict[fish->myScoreMult] //value for spritids[3] whenever I get scoremult in
    //These two blocks flash between the fish's skin and the score
    if (fish->respawnTimer%2 == 0 && fish->animDelayer <= 0){
        set_sprite_tile(fish->spritids[0], numSprDict[fish->scoreValue/10]);
        set_sprite_tile(fish->spritids[1], numSprDict[fish->scoreValue%10]);
        set_sprite_tile(fish->spritids[2], 65); //This should be an X
        set_sprite_tile(fish->spritids[3], numSprDict[fish->myScoreMult]); //Maybe this is 0 because Score mult is global so they're all picking up after it's been reset?
        }
    else if (fish->respawnTimer%2 != 0 && fish->animDelayer <= 0){
        set_sprite_tile(fish->spritids[0], fish->currentSkin);
        set_sprite_tile(fish->spritids[1], fish->currentSkin+1);
        set_sprite_tile(fish->spritids[2], fish->currentSkin+2);
        set_sprite_tile(fish->spritids[3], fish->currentSkin+3);
        }

    //Fish values are reset and their location is randomized when the animation is over
    else if (fish->respawnTimer <= 0){
        fish->respawnTimer = 0; //make sure you can hit respawned fish with net
        fishpicker(fish, fishSkin);
        fish->flippedForDeath = 0;
        fish->xLoc = randomNumberGenerator(161, 11); //11 is the amount of tiles that exist off-screen and 160 is the first tile where fish aren't visible
        fish->yLoc = randomNumberGenerator(81, 56); //56 potential pixels where fish is allowed to be. 80 is the first one
    }
    //SWITCH_ROM(0);

    //Decrementing timer values where appropriate
    if (fish->animDelayer <= 0){
        fish->animDelayer = 5;
        fish->respawnTimer --;
    }
    fish->animDelayer --; 
}

#endif /* UNBANKEDFUNCTIONS_H */