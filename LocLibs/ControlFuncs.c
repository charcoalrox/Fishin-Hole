///Functions that control movement and actions of fish and player character. Banked

#pragma bank 1

#include <gb/gb.h>
#include "../bgmdat/gbt_player.h"

extern void snd_pauseOff(void);
extern void snd_pauseOn(void);

extern void performantdelay(uint8_t timeDelayed);

//utilizing structs in alternative bank code
extern struct GameCharacter{
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
    UBYTE flippedForDeath;
}GameCharacter;


extern struct FishProfile{
//Setting every type of fish on startup so fish can respawn into new fish when they're caught
    UBYTE sprID[4];
    UBYTE palID[4];
    uint8_t speed;
    uint8_t scoreVal;
}FishProfile;


extern struct GameCharacter man;
extern struct GameCharacter fish1;
extern struct GameCharacter fish2;
extern struct GameCharacter fish3;
extern struct GameCharacter fish4;
extern struct FishProfile snout;
extern struct FishProfile pink;
extern struct FishProfile puffer;
extern struct FishProfile clown;
extern struct FishProfile Angel;


extern uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt);

const UBYTE spritesize = 8;
extern UBYTE isPressed;


//These frankly don't belong here but I didn't want a whole new page for a single thing so they live here anyways
//All of the phrases that the blue screen can flash
//I couldn't think of a better word than death threat. Phrases encouraging suicide as per the original text
char dThreat0[] = "freedom";
char dThreat1[] = "do it now";
char dThreat2[] = "tell your friends";
char dThreat3[] = "eternal rest";
char dThreat4[] = "visit ztheend";
char dThreat5[] = "take the plunge";
char dThreat6[] = "i understand";
char dThreat7[] = "no more struggle";
char dThreat8[] = "dont be afraid";
char dThreat9[] = "join the movement";
char dThreat10[] = "find happiness";

char *dThreatArray[] = {dThreat0, dThreat1, dThreat2, dThreat3, dThreat4, dThreat5, dThreat6, dThreat7, dThreat8, dThreat9, dThreat10};


void objectflip(int8_t directionFromSpeed, struct GameCharacter* character, uint8_t x, uint8_t y){
//Faces a sprite the direction they're moving. To be called inside of movegamecharacter and not outside of that function
    uint8_t spriteshift = 8; //sprites are mirrored to proper side if object is facing left or right

    if (directionFromSpeed < 0){ //if you're moving right, flip
        spriteshift = 0;
        set_sprite_prop(character->spritids[0], character->palids[0] | get_sprite_prop(character->spritids[0]) & ~S_FLIPX);
        set_sprite_prop(character->spritids[1], character->palids[1] | get_sprite_prop(character->spritids[1]) & ~S_FLIPX);
        set_sprite_prop(character->spritids[2], character->palids[2] | get_sprite_prop(character->spritids[2]) & ~S_FLIPX);
        set_sprite_prop(character->spritids[3], character->palids[3] | get_sprite_prop(character->spritids[3]) & ~S_FLIPX);
    }
    else{
        set_sprite_prop(character->spritids[0], character->palids[0] | S_FLIPX);
        set_sprite_prop(character->spritids[1], character->palids[1] | S_FLIPX);
        set_sprite_prop(character->spritids[2], character->palids[2] | S_FLIPX);
        set_sprite_prop(character->spritids[3], character->palids[3] | S_FLIPX);
    }
    //Moves sprites to correct side of meta sprite based on flip
    move_sprite(character->spritids[0], x+spriteshift, y);
    move_sprite(character->spritids[1], x-spriteshift + spritesize, y);
    move_sprite(character->spritids[2], x+spriteshift, y + spritesize);
    move_sprite(character->spritids[3], x-spriteshift + spritesize, y + spritesize);
}


void movegamecharacter(struct GameCharacter* character, uint8_t x, uint8_t y, int8_t speed){
//Move 16 pixel sprites in any cardinal direction. Automatically flips sprite on x/y axis
    character->xLoc += speed;

    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x + spritesize, y);
    move_sprite(character->spritids[2], x, y + spritesize);
    move_sprite(character->spritids[3], x + spritesize, y + spritesize);

    objectflip(speed, character, character->xLoc, character->yLoc);
}


void setupgamecharacter(struct GameCharacter* person, uint8_t sprID, uint8_t sprite, uint8_t palID, uint8_t xLoc, uint8_t yLoc, uint8_t Xdir, uint8_t Ydir, uint8_t scoreValue, uint8_t currentSkin){
//Create game character structs (fish and human. Not submarine) in RAM
    person->xLoc = xLoc;
    person->yLoc = yLoc;
    person->width = 16;
    person->height = 16;
    person->scoreValue = scoreValue;
    person->Xdir = Xdir;
    person->Ydir = Ydir;
    person->selectMovement = 1; //default movement type
    person->currentSkin = currentSkin;
    person->respawnTimer = 0; //Respawn timer must be reset with game character or collision breaks
    person->flippedForDeath = 0;

    for (int i=0; i<4; i++){
        set_sprite_tile(sprID + i, sprite + i);
        person->spritids[i] = sprID + i;
        person->palids[i] = palID;
        set_sprite_prop(person->spritids[i], palID);
    }

    

    movegamecharacter(person, person->xLoc, person->yLoc, person->Xdir);
}


void fishprofileinit(struct FishProfile* FishData, uint8_t sprID, uint8_t palID, uint8_t speed, uint8_t scoreVal){
//Create profile data for all types of fish. Allows us to swap fish into new fish on demand later

    for (int i=0; i<4; i++){
        FishData->sprID[i] = sprID + i;
        FishData->palID[i] = palID;
    }

    FishData->speed = speed;
    FishData->scoreVal = scoreVal;
}


//Don't bank so I can call it in conditionals
UBYTE checkCollisions(uint8_t netLocX, uint8_t netLocY, uint8_t objLocX, uint8_t objLocY, uint8_t objWidth, uint8_t objHeight, int8_t handicap){
//Check collision of a one-sprite object with a two-sprite object (mainly the net against fish)

    return 
    (netLocX >= objLocX-handicap && netLocX <= objLocX + objWidth+handicap) &&  //one.x is less than two.x and one.x less than two.xplus two.width means the width of one is within the width of two
    (netLocY >= objLocY && netLocY <= objLocY + objHeight)

    ||

    (objLocX >= netLocX-handicap && objLocX <= netLocX + 8+handicap) &&
    (objLocY >= netLocY && objLocY <= netLocY + 8);
}


void fishpicker(struct GameCharacter* fish, struct FishProfile *fishSkin){
//Can be called to turn any gamecharacter object into a randomized fish (should only be called on fish, obviously)    
    for (int i=0; i<4; i++){
        set_sprite_tile(fish->spritids[i], fishSkin->sprID[i]);
        fish->palids[i] = fishSkin->palID[i];
        set_sprite_prop(fish->spritids[i], fish->palids[i]);
    }

    fish->currentSkin = fishSkin->sprID[0];
    fish->scoreValue = fishSkin->scoreVal;
    fish->Xdir = fishSkin->speed;
}


void fishBounce(struct GameCharacter* fish, uint8_t ySpeed, uint8_t yMin, uint8_t yMax, uint8_t pauseLen){
//Fish moves in a zig-zag motion. Specified from source material. 

    if (fish->timer > 0){
        fish->timer--;
    }
    else if (fish->timer <= 0){
        fish->yLoc += ySpeed * fish->Ydir;

        movegamecharacter(fish, fish->xLoc, fish->yLoc, fish->Xdir);

        if (fish->yLoc <= yMin || fish->yLoc >= yMax){
            fish->Ydir *= -1;
            fish->timer = pauseLen;
        }
    }
}

//pick a random skin BEFORE we respawn. This will appease the compiler gods
uint8_t fishBehavior(struct GameCharacter* fish, struct FishProfile* fishSkin){
//Randomly chooses what fish will do at edge of tank
//Return value decides if the fish should move straight or in zig-zags

    if ((fish->xLoc >= 167) || fish->xLoc <= 0){ //167, 1 original values
        switch(randomNumberGenerator(0, 12)){ 
            case 0: //Normal Movement
            case 1:
            case 2:
            case 3:
                return 1;
                break;
            case 4: //Move up and down
            case 5:
            case 6:
                return 0;
                break;
            case 7: //Fish spawns as a new type of fish. Always pink if isharmode==1
                fishpicker(fish, fishSkin);
                return 1;
                break;
            case 8: //Fish moves to a new y-coordinate
            case 9:
            case 10:
                movegamecharacter(fish, fish->xLoc, randomNumberGenerator(32, 48), fish->Xdir);
                return 1;
                break;
            default: //Fish turns around
                fish->Xdir *= -1;
                return 1;
                break;
        }
    }
    return fish->selectMovement; //Compiler requires a return but setting this to anything will break the funcition
}


uint8_t fishBBallhavior(uint8_t selectMovement, uint8_t randomNumber){

    if ((fish1.xLoc >= 159) || fish1.xLoc <= 24){

        fish1.Xdir *= -1;

        //Stops fish from getting trapped behind the walls (my job)
        if (fish1.xLoc >= 159){
            fish1.xLoc -= 1;
        }
        else if (fish1.xLoc <=24){
            fish1.xLoc += 1;
        }

        switch (randomNumber)
        {
        case 0: //move normally 
        case 1:
        case 2:
        case 3:
            return 1;
        case 4: //fish zigzag
        case 5:
        case 6:
            return 0;
        default: //move normally (again)
            return 1;      
        }
    }
    return selectMovement;
}


uint8_t pause_game(void){
//freezes game if you hit start. Eventually it should also return to menu if you press select afterwards

    isPressed = 1; //I set button pressed to a global on this page for some reason so this won't be consistently named
    performantdelay(5);

    snd_pauseOn();
    SHOW_WIN;
    while(1){
        if (!(joypad() & J_START)){
            isPressed = 0;
        }
        if (joypad() & J_START && isPressed == 0){
            HIDE_WIN;
            snd_pauseOff();
            gbt_pause(1);
            return 0;
        }
        if (joypad() & J_SELECT){ //Flag so you can quit games with start+select
            HIDE_WIN;
            return 1;
        }
        gbt_pause(0);
    }
}