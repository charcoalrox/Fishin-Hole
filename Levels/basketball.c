#pragma bank 1

#include "gb/gb.h"
//#include "../LocLibs/GameCharacter.h"
//#include "../bgmdat/gbt_player.h"

#define BackgroundWidth 20
#define BackgroundHeight 18
#define BackgroundBank 0

#define Background BackgroundPLN0
extern unsigned char BackgroundPLN0[];
extern unsigned char BackgroundPLN1[];

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
}GameCharacter;


//I need the structs for this anyways. Why not put the finished product here?
extern struct GameCharacter man;
extern struct GameCharacter fish1;
extern struct FishProfile puffer;

extern void snd_dropnet(void);
extern void snd_hitsub(void);
extern void snd_hitfish(void);

extern void setupgamecharacter(struct GameCharacter* person, uint8_t sprID, uint8_t sprite, uint8_t palID, uint8_t xLoc, uint8_t yLoc, uint8_t Xdir, uint8_t Ydir, uint8_t scoreValue, uint8_t currentSkin);
extern void objectflip(int8_t directionFromSpeed, struct GameCharacter* character, uint8_t x, uint8_t y);
extern void hudelementsinit(void);
extern const uint8_t numSprDict[10];
extern uint8_t movearrow(uint8_t arrowY);
extern uint8_t adjustarrow(uint8_t arrowX, uint8_t arrowY, uint8_t arrowYMiddleVal);
extern void player_thrownet(void);
extern void player_reset(void);
extern UBYTE checkCollisions(uint8_t netLocX, uint8_t netLocY, uint8_t objLocX, uint8_t objLocY, uint8_t objWidth, uint8_t objHeight, int8_t handicap);
extern void updatescore(uint8_t numAdded, uint8_t scoreMult);
extern uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt);
extern void movegamecharacter(struct GameCharacter* character, uint8_t x, uint8_t y, int8_t speed);
extern void fishBounce(struct GameCharacter* fish, uint8_t ySpeed, uint8_t yMin, uint8_t yMax, uint8_t pauseLen);
extern void performantdelay(uint8_t timeDelayed);
extern uint8_t pause_game(void);

extern void picksong(unsigned char songName[], uint8_t songSpeed);
extern const unsigned char * const BytheSea_Data[];

extern void setupBasketballGame(void);
extern uint8_t fishBBallhavior(uint8_t selectMovement, uint8_t randomNumber);

extern UBYTE isPressed;


void lvl_basketball(void){
    DISPLAY_OFF;
     
    setupBasketballGame();

    setupgamecharacter(&fish1, 5, 13, 2, 80, 120, 3, 1, 30, 5);
     
    uint8_t selectMovement = 1; //Describes fish movement in fish movement function 1 means it's moving back and forth normally

    //Timer variables
    uint8_t timerDelayer = 25; //must exist outside of timersetter func so that time can be constant and saved
    uint8_t timerArray[3] = {1, 0};
    uint8_t timeRemaining = 30;

    //Arrow movement and setup. number value of net is set up so that it matches where it is in main game on my sprite key
    uint8_t arrowX = 40;
    const uint8_t arrowYMiddleValue = 50;
    uint8_t arrowY = arrowYMiddleValue;
    set_sprite_prop(38, 3);
    move_sprite(38, arrowX, arrowY);

    //Basketball variables. They're all still net because I blatantly copy-pasted this code
    uint8_t netDelay = 0; //delays the falling speed of the net so that I can make it less than 1
    uint8_t netVelMax = 15;
    uint8_t netVelX = netVelMax;
    int8_t netVelY = 0;
    int8_t netInitPosition = 0;

    UBYTE netActive = 0;
    uint8_t netLocX = 0;
    uint8_t netLocY = 0;

    set_sprite_tile(35, 84);
    set_sprite_prop(35, 4);

    //MakingBasketballHoop. Because it reuses some sprites there is NOT a better way to do this
    set_sprite_tile(10, 85); //Hoop pole
    set_sprite_tile(11, 85); //hoop pole
    set_sprite_tile(12, 86); //Hoop bottom left
    set_sprite_tile(13, 87); //Hoop bottom right
    set_sprite_tile(14, 90); //Hoop middle left
    set_sprite_tile(15, 91); //hoop middle right
    set_sprite_tile(16, 88); //Hoop top left
    set_sprite_tile(17, 89); //hoop top right

    //SR2;
    //picksong(BytheSea_Data, 3);
    isPressed == 1;
     

    DISPLAY_ON;

    while(1){

         
        //Pause game
        if (joypad() & J_START && isPressed == 0){
            if (pause_game() == 1){
                isPressed = 1;
                break;

            }
            isPressed = 1;
        }

        //Decrement timer. End game if timer reaches 0
        timerDelayer--;

        set_sprite_tile(33, numSprDict[timeRemaining/10]); //convert the 1's and 10's place from timerDelay into sprites on the screen
        set_sprite_tile(34, numSprDict[timeRemaining%10]);
         

        if (timerDelayer == 0){ //When the counter delayer signals, the normal counter is decremented. If the game timer reaches 0, we kill it
            timeRemaining--;

            if (timeRemaining <= 0){
                break;
            }
        }

        if (timerDelayer <= 0){ //Counting down once per frame is too fast. We put this here to delay that
            timerDelayer = 25;
        }

        //Adjust player's arrow movement
         
        arrowY = movearrow(arrowY);
        arrowX = adjustarrow(arrowX, arrowY, arrowYMiddleValue);
        move_sprite(38, arrowX, arrowY);
         

        //Drop basketball. Unfortunately this is connected to a lot of local variables so I can't make it a function. The ugly spaghetti stays
        if(joypad() & J_A){

            if (netActive == 0 & isPressed == 0){
                 
                player_thrownet();
                snd_dropnet();
                 

                netVelY = netInitPosition;

                //Set initial arrow velocity depending on arrow position to make more intuitive arcs
                if (arrowY <= 44){
                    netInitPosition = 1;
                    netVelX = netVelMax + netInitPosition - 2;
                }
                else if (arrowY >44 && arrowY < 54){
                    netInitPosition = 0;
                    netVelX = 12;
                }
                else if (arrowY >= 54){
                    netInitPosition = -8;
                    netVelX = 10;
                }
                netLocY = arrowY;
                netVelY = netInitPosition;
                netLocX = arrowX;
                move_sprite(35, netLocX+4, netLocY); //man's sprites are 8x8. +4 to be in the middle of x and +16 to be under BOTH sprites. Obviously this needs to be updated since we throw from arrow now

                netActive = 1;
            }
        isPressed = 1;
        }

        if(!(joypad() & J_A) && !(joypad() & J_START)){
            isPressed = 0;
        }

        if (netActive == 1){
            netLocY -= netVelY;
            if (netVelY > -6){ //makes net fall at a more realistic pace. Lower numbers = faster
                netVelY --;
            }
            if (netVelX > 0){
                netLocX += netVelX;
                netVelX --;
            }
            move_sprite(35, netLocX, netLocY);
        }

        //Set player sprite to default and reset net to dock near player until thrown again if net hits ground or hoop
        if(netLocY > 160 && netActive == 1){ 
            netVelX = netVelMax;
            netActive = 0;
             
            snd_hitsub();
            player_reset();
             
            netLocX = 11;
            netLocY = 56;
            move_sprite(35, netLocX, netLocY);
        }

        for (int i=0; i<8; i++){
            set_sprite_prop(i+10, 4);
        }

        move_sprite(10, fish1.xLoc+8, fish1.yLoc-8); //pole
        move_sprite(11, fish1.xLoc+8, fish1.yLoc-16); //pole
        move_sprite(12, fish1.xLoc, fish1.yLoc-24); //hoop bottom left
        move_sprite(13, fish1.xLoc+8, fish1.yLoc-24); //hoop bottom right
        move_sprite(14, fish1.xLoc, fish1.yLoc-32);
        move_sprite(15, fish1.xLoc+8, fish1.yLoc-32);
        move_sprite(16, fish1.xLoc, fish1.yLoc-40);
        move_sprite(17, fish1.xLoc+8, fish1.yLoc-40);

        //Collision with net
            //If the fish is hit by a net for the first time between respawns

         
        if (checkCollisions(netLocX, netLocY, fish1.xLoc, fish1.yLoc-32, fish1.width, fish1.height, 5) && netActive == 1){ //real shit math checking if the ball hits the net or backboard
            snd_hitfish();
            netActive = 0;
            updatescore(1, 1);

            player_reset();
            netLocX = 11; //reset ball loc
            netLocY = 56;
            move_sprite(35, netLocX, netLocY);
            }
        
        //Fish movement
        selectMovement = fishBBallhavior(selectMovement, randomNumberGenerator(0, 9));
        if (selectMovement == 1){
            movegamecharacter(&fish1, fish1.xLoc, fish1.yLoc, fish1.Xdir);
        }
        if (selectMovement == 0){
            fishBounce(&fish1, 5, 96, 136, 3);
        }

        performantdelay(3);
         
        //gbt_update();
    }
}