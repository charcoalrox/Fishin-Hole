#include "gb/gb.h"
#include "stdio.h"
#include "../ImgCode/BackgroundData/background.h"
#include "../LocLibs/GameCharacter.h"
#include "../bgmdat/gbt_player.h"

extern void snd_hitfish(void);
extern void snd_dropnet(void);
extern void snd_hitsub(void);

extern void setupgamecharacter(GameCharacter* person, uint8_t sprID, uint8_t sprite, uint8_t palID, uint8_t xLoc, uint8_t yLoc, uint8_t Xdir, uint8_t Ydir, uint8_t scoreValue, uint8_t currentSkin);
extern void objectflip(int8_t directionFromSpeed, GameCharacter* character, uint8_t x, uint8_t y);
extern void hudelementsinit(void);
extern void fishprofileinit(struct FishProfile* FishData, uint8_t sprID, uint8_t palID, uint8_t speed, uint8_t scoreVal);
extern const uint8_t numSprDict[10];
extern void fishpicker(struct GameCharacter* fish, struct FishProfile* fishSkin);
extern uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt);
extern void movegamecharacter(GameCharacter* character, uint8_t x, uint8_t y, int8_t speed);
extern void performantdelay(uint8_t timeDelayed);
extern UBYTE checkCollisions(uint8_t netLocX, uint8_t netLocY, uint8_t objLocX, uint8_t objLocY, uint8_t objWidth, uint8_t objHeight, int8_t handicap);
extern uint8_t score[6];
extern void updatescore(uint8_t numAdded, uint8_t scoreMult);
extern uint8_t pause_game(void);
extern void player_thrownet(void);
extern void player_reset(void);

extern void setupFishMode(void);

extern void init_all_profiles(void);

extern void picksong(unsigned char songName[], uint8_t songSpeed);
extern const unsigned char * const BytheSea_Data[];

extern UBYTE isPressed;


void lvl_fishmode(void){
    SR1;
    setupFishMode();

    init_all_profiles();

    setupgamecharacter(&fish1, 6, 5, 1, 80, 120, 2, 1, 99, 1); //Gibberish values because they're randomized right afterwards (other than sprite id and location)
    fishpicker(&fish1, skinList[randomNumberGenerator(0, 4)]);
    movegamecharacter(&fish1, fish1.xLoc, fish1.yLoc, -3);
    SR0;

    //variables for net
    uint8_t netSpeed = 2; //Net's speed for falling and moving. Will slowly increment
    const uint8_t netBaseX = 23;
    const uint8_t netBaseY = 42;

    uint8_t netCoordinates[] = {netBaseX, netBaseY}; //Divisible by 3 so that fish can move at speed of 3
    uint8_t netTarget = 80; //This is going to save the fish's x-coordinate when the net is thrown. Makes net more realistic and makes higher scores possible
    UBYTE netIsFalling = 0;
    uint8_t netLocX = 0;
    int8_t netLocY = 0;

    UBYTE correctSprite = 0; //Stops player_reset from being called on a loop forever

    int8_t direction = 1;

    //Makes submarine
    set_sprite_tile(36, 69);
    set_sprite_prop(36, 7);
    set_sprite_tile(37, 70);
    set_sprite_prop(37, 7);

    //Makes second submarine
    set_sprite_tile(11, 70);
    set_sprite_prop(11, 7 | S_FLIPX);
    set_sprite_tile(12, 69);
    set_sprite_prop(12, 7 | S_FLIPX);

    int8_t submarine2X = 0;
    uint8_t submarine2Y = 90;

    int8_t submarineX = 120;
    uint8_t submarineY = 150;
    uint8_t subSpeed = 1;
    move_sprite(36, submarineX, submarineY);
    move_sprite(37, submarineX + 8, submarineY);

    SR2;
    picksong(BytheSea_Data, 2);
    SR0;

    while(1){

        SR1;
        //Pause game
        if (joypad() & J_START && isPressed == 0){
            if (pause_game() == 1){
                isPressed = 1;
                break;

            }
            isPressed = 1;
        }

        if (!(joypad() & J_START)){
            isPressed = 0;
        }

        //Player movement
        if(joypad() & J_LEFT && fish1.xLoc> 34){
            movegamecharacter(&fish1, fish1.xLoc, fish1.yLoc, -3);
            direction = -1;
        }

        if(joypad() & J_RIGHT && fish1.xLoc< 152){
            movegamecharacter(&fish1, fish1.xLoc, fish1.yLoc, 3);
            direction = 1;
        }

        if (joypad() & J_UP && fish1.yLoc > 96){
            fish1.yLoc-=3;
            movegamecharacter(&fish1, fish1.xLoc, fish1.yLoc, 0);
        }

        if (joypad() & J_DOWN && fish1.yLoc < 136){
            fish1.yLoc+=3;
            movegamecharacter(&fish1, fish1.xLoc, fish1.yLoc, 0);
        }
        objectflip(direction, &fish1, fish1.xLoc, fish1.yLoc); //fix bug where fish faced wrong way when going vertically and to the right
        SR0;

        //Net arcs upwards until it's above fish. Then it falls
        if (netTarget <= netCoordinates[0] || netCoordinates[0] > 175){ //If the net is over the fish or misses the fish and makes it out too far
            netIsFalling = 1;
        }
        if (netIsFalling == 0){
            netCoordinates[0]+=netSpeed * 2;

        //Simulates the arc of a net. attempts to have a middly ground to travel straight. Makes a cup shape
            if(netCoordinates[0] <= (netTarget/2)-3){
                netCoordinates[1]-=netSpeed;
            }
            if(netCoordinates[0] > (netTarget/2)+20){
                netCoordinates[1]+=netSpeed*3;
            }
            //if (netCoordinates[1] > 30){
            //    netCoordinates[1]-=netSpeed;
            //}
            move_sprite(5, netCoordinates[0], netCoordinates[1]);
        }
        SR1;
        if (netIsFalling == 1){
            if (correctSprite == 0){ //stops hitsub noise from playing on loop forever
                player_reset();
                correctSprite = 1;
            }
            netCoordinates[1]+=netSpeed*3;
            move_sprite(5, netCoordinates[0], netCoordinates[1]);
            
        }

        if (netCoordinates[1] > 160 || checkCollisions(netCoordinates[0], netCoordinates[1], submarineX, submarineY, 16, 8, 2) || checkCollisions(netCoordinates[0], netCoordinates[1], submarine2X, submarine2Y, 16, 8, 2)){ //If the net hits the ground. Sub code don't work :C
            snd_hitsub();
            if (netIsFalling == 1){ //Odd setup to make sure score is only += 1. We also needed to change netIsFalling anyways
                netIsFalling = 0;
                updatescore(1, 1);
            }
            netCoordinates[0] = netBaseX;
            netCoordinates[1] = netBaseY;
            netTarget = fish1.xLoc;
            if (netSpeed < 5 && score[5] == 0){ //difficulty increases every 10 points until it's 5* faster than original
                netSpeed ++;
                subSpeed ++;
                if (score[4] > 5){ //submarines go 3x faster if player score is greater than 50
                    subSpeed = 3;
                }
            }
            
            player_thrownet();
            correctSprite = 0;
            snd_dropnet();
        }

        //Player death :C
        if (checkCollisions(netCoordinates[0], netCoordinates[1], fish1.xLoc, fish1.yLoc, 16, 16, 0) || 
        checkCollisions(fish1.xLoc, fish1.yLoc, submarineX, submarineY, 16, 8, 0) || 
        checkCollisions(fish1.xLoc+8, fish1.yLoc, submarineX, submarineY, 16, 8, 0) ||
        checkCollisions(fish1.xLoc, fish1.yLoc+8, submarineX, submarineY, 16, 8, 0) ||
        checkCollisions(fish1.xLoc+8, fish1.yLoc+8, submarineX, submarineY, 16, 8, 0) ||

        checkCollisions(fish1.xLoc, fish1.yLoc, submarine2X, submarine2Y, 16, 8, 0) || 
        checkCollisions(fish1.xLoc+8, fish1.yLoc, submarine2X, submarine2Y, 16, 8, 0) ||
        checkCollisions(fish1.xLoc, fish1.yLoc+8, submarine2X, submarine2Y, 16, 8, 0) ||
        checkCollisions(fish1.xLoc+8, fish1.yLoc+8, submarine2X, submarine2Y, 16, 8, 0)){ //If the net or either sub touches you. You LOSE. V sorry it looks like this collision was built for a net versus a fish
        //snd_hitfish();
            break;
        }

        submarineX-=subSpeed;
        move_sprite(36, submarineX, submarineY);
        move_sprite(37, submarineX + 8, submarineY);

        if (score[4] >= 3){ //Sub 2 only activates with scores greater than 30
            submarine2X+=subSpeed;
            move_sprite(11, submarine2X, submarine2Y);
            move_sprite(12, submarine2X + 8, submarine2Y);
        }

        const uint8_t submarineYOffset = 100;
        
        if (submarineX+8 == 0){
            uint8_t subRandY = randomNumberGenerator(0, 82);
            submarineY = subRandY + submarineYOffset;
        }

        if (submarine2X == 0){
            uint8_t subRandY = randomNumberGenerator(0, 82);
            submarine2Y = subRandY + submarineYOffset;
        }

        performantdelay(3);
        SR0;
        gbt_update();
    }
}