#include "gb/gb.h"
#include "stdio.h"
#include "../ImgCode/BackgroundData/background.h"
#include "../LocLibs/GameCharacter.h" //Fish Structs
#include "../ImgCode/HUDScreens/BlueFlashMap.c"
#include "../ImgCode/HudScreens/HUDCleaner.c"
#include "../LocLibs/unbankedfuncs.h"
#include "../bgmdat/gbt_player.h"

#define EXIT_BUTTONS_UNBIASED (joypad() & J_START || joypad() & J_SELECT || joypad() & J_B) && isPressed == 0 //These factors will always decide if demo screen is exited

extern void snd_hitfish(void);
extern void  snd_hitsub(void);

extern char *dThreatArray[11];
extern uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt);
extern void setupgamecharacter(GameCharacter* person, uint8_t sprID, uint8_t sprite, uint8_t palID, uint8_t xLoc, uint8_t yLoc, uint8_t Xdir, uint8_t Ydir, uint8_t scoreValue, uint8_t currentSkin);
extern void fishprofileinit(struct FishProfile* FishData, uint8_t sprID, uint8_t palID, uint8_t speed, uint8_t scoreVal);
extern UBYTE checkCollisions(uint8_t netLocX, uint8_t netLocY, uint8_t objLocX, uint8_t objLocY, uint8_t objWidth, uint8_t objHeight, int8_t handicap);
extern uint8_t fishBehavior(struct GameCharacter* fish, struct FishProfile* fishSKin);
extern void fishBounce(struct GameCharacter* fish, uint8_t ySpeed, uint8_t yMin, uint8_t yMax, uint8_t pauseLen);

extern void text_to_hud(char text[], unsigned char hud[], uint8_t x, uint8_t y, uint8_t w, uint8_t h);
extern void resetBackground(char background[]);

extern void picksong(unsigned char songName[], uint8_t songSpeed);
extern const unsigned char * const BytheSea_Data[];


extern void init_all_profiles(void);
extern void init_all_fish(uint8_t isHardMode, UBYTE makeProfiles);
extern void setupDemo(void);

extern UBYTE isPressed;


void lvl_demo(void){
    DISPLAY_OFF;
    SR1;
    setupDemo();

    set_sprite_tile(0, 83);
    move_sprite(0, 0, 0);

    UBYTE isDemoActive = 0;

    uint8_t cursor_X = 96;
    uint8_t cursor_Y = 96;

    init_all_fish(0, 1);

    set_win_tiles(0, 0, 14, 2, HUDCleaner);//Sets the text on the pause menu section of the HUD to be blank

    performantdelay(5); //Select exits demo screen. Making life easier for people who accessed this screen via the select button
    SR0;

    SR2;
    picksong(BytheSea_Data, 2);
    SR0;

    isPressed = 1;

    DISPLAY_ON;

    while(1){

        if (joypad() & J_UP){
            cursor_Y -=2;
            move_sprite(0, cursor_X, cursor_Y);
        }
        if (joypad() & J_DOWN){
            cursor_Y +=2;
            move_sprite(0, cursor_X, cursor_Y);
        }
        if (joypad() & J_LEFT){
            cursor_X -=2;
            move_sprite(0, cursor_X, cursor_Y);
        }
        if (joypad() & J_RIGHT){
            cursor_X +=2;
            move_sprite(0, cursor_X, cursor_Y);
        }

        SR1;
        if ((joypad() & J_START || joypad() & J_SELECT || joypad() & J_B || ((joypad() & J_A) && (cursor_X == 96 && cursor_Y == 96))) && isPressed == 0){
        //if buttons not used for anything else are pressed and not held down from other screen, go back to main menu. Makes sure A also works but only if the arrow hasn't been moved yet
            resetBackground(Background);
            break;
        }

        for (int i=0; i<4; i++){
            if ((joypad() & J_A) && checkCollisions(cursor_X+4, cursor_Y, (*fishList[i]).xLoc, (*fishList[i]).yLoc, 16, 16, 0) && (*fishList[i]).currentSkin == 9 && isPressed == 0 && (*fishList[i]).Xdir != 0){
                snd_hitfish();
                isPressed = 1;
                (*fishList[i]).Xdir = 0;
                (*fishList[i]).Ydir = 0;
                (*fishList[i]).scoreValue = randomNumberGenerator(0, 99);
                (*fishList[i]).animDelayer = 1;
                (*fishList[i]).myScoreMult = 0;
                (*fishList[i]).respawnTimer = 5;
                (*fishList[i]).flippedForDeath = 0;
            }

            if ((*fishList[i]).Xdir == 0 && (*fishList[i]).xLoc != 0){
                fishDeathAnim(&(*fishList[i]), skinList[randomNumberGenerator(0, 4)]);
            }
            else if ((joypad() & J_A) && checkCollisions(cursor_X+4, cursor_Y, (*fishList[i]).xLoc, (*fishList[i]).yLoc, 16, 16, 0) && (*fishList[i]).currentSkin != 9  && isPressed == 0){  //Check ispressed on this when I get more space
                snd_hitsub();
                isPressed = 1;
            }

            (*fishList[i]).selectMovement = fishBehavior(&(*fishList[i]), skinList[randomNumberGenerator(0, 4)]);

            if ((*fishList[i]).Xdir != 0){ //Stops movement from auto-flipping fish backwards after they die
                if ((*fishList[i]).selectMovement == 0){
                    fishBounce(&(*fishList[i]), 5, 80, 136, 3);
                }
                else
                {
                    movegamecharacter(&(*fishList[i]), (*fishList[i]).xLoc, (*fishList[i]).yLoc, (*fishList[i]).Xdir);
                }
            }

        }
        SR0;

        if ((cursor_X != 96 || cursor_Y != 96) && isDemoActive == 0){
        //The book says the lyrics of the song become evil when the demo screen is hartsfield'd. Best I can do on the hardware
            SR2;
            picksong(BytheSea_Data, 5);
            SR0;

            isDemoActive = 1;
        }

        SR1;
        uint16_t willFlash = randomNumberGenerator(0, 400);
        if (willFlash == 3 && (cursor_X != 96 || cursor_Y != 96)){ //Blue flashes run every 10-50 seconds but only after you activated the cursor

            //Random phrase assigned to blue flash
            uint8_t randomIndex = randomNumberGenerator(0, 11);
            char *text = dThreatArray[randomIndex]; //Global array of messages

            //Write blue flash to screen. Display for a split second
            text_to_hud(text, BlueFlashMap, 2, 8, 17, 1);
            performantdelay(5); //window only flashes on screen long enough to glance at text here
            HIDE_WIN;
        }

        if (!(joypad() & J_SELECT) && !(joypad() & J_A)){
            isPressed = 0;
        }

        performantdelay(3);
        SR0;
        gbt_update();
    }
}