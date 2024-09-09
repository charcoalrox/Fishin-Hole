///Calls demo screen without subliminal messages


#include <gb/gb.h>
#include <gb/cgb.h>
#include "ImgCode/paletteData.c" //Not in the make file. Causes include errors
#include "Imgcode/GameSprites.h" //Sprite tilesheet as code
#include "ImgCode/MapTileSet.h" //Tileset for background
#include "ImgCode/BackgroundData/BG_StartMenu.h" //Actual background file
#include "Levels/SubGames.c"
#include "Levels/MainGame.c"
#include "Levels/DemoScreenClean.c"
#include "bgmdat/gbt_player.h"
#include "ImgCode/HUDScreens/HudCleaner.c"
#include "LocLibs/GameCharacter.h"
#include <stdio.h>


#define PREPARE_NEW_LEVEL SR1; snd_hitfish(); clear_OAM(); clearHighscore(); SR0;
#define RESET_SPRITE_POS cursorYPos = 88; move_sprite(0, 8, 88); boatPosX = 35; boatSpriteShift = 0; boatDir = -1;//I can't reset these local variables in a function cleanly so I'm making a macro to help do it prettier
#define RESET_MENUSCREEN DISPLAY_OFF; SR1; clear_OAM(); SWITCH_ROM(0); RESET_SPRITE_POS; SR1; reloadMainMenu(); SR0; DISPLAY_ON; isPressed = 1;
#define MOVE_BOAT boatPosX += boatDir; move_sprite(1, boatPosX + boatSpriteShift, 151); move_sprite(2, boatPosX + 8 - boatSpriteShift, 151);


UBYTE isPressed = 0; //Global used on all screens with a function to decide if an input is pressed. Stops input from affecting multiple screens at once

const uint8_t delay_Time = 6;


//High scores stored in 2d arrays. Ported from savestate.c
extern uint8_t *main_HighScores[12];
extern uint8_t *hard_HighScores[12]; //hard mode scores
extern uint8_t *basketball_HighScores[12]; //basketball scores
extern uint8_t *fishmode_HighScores[12];

extern void scoreInitializer(void);

//Banked Functions
extern uint8_t menutraversal(uint8_t button1Pos, uint8_t cursorIDNO, uint8_t numOfButtons, uint8_t menuSelectorX, uint8_t menuSelectorY);
extern void performantdelay(uint8_t timeDelayed);
extern void clear_OAM(void);
extern void snd_quack(void);

extern void clearHighscore(void);

extern void snd_hitfish(void);

extern void set_hudmap_demo(void);
extern void reloadMainMenu(void);

extern int hasnewhighscore(uint8_t *TwoD_Array[]);
extern void lvl_HighScore(uint8_t *TwoD_Array[], UBYTE fromMain);

extern unsigned char HUDMap[28];

extern void checkifPressed(uint8_t gamepadInput);


void main(void){

    //Initialize color palettes
    set_bkg_palette(0, 8, &backgroundpalette[0]);
    set_sprite_palette(0, 8, &spritepalette[0]);

    //Initialize sprites (todo: Optimize)
    set_sprite_data(0, 93, GameSprites);
    set_bkg_data(0, 128, MapTileSet); 

    //Setup main menu sprites and menu positions
    SR1;
    delay(1);
    reloadMainMenu();
    scoreInitializer(); //Score intiializers set the high scores to their default scores IF the save data is blank
    SR0;

    uint8_t boatPosX = 35;
    uint8_t boatDelayCounter = 0;
    int8_t boatDir = -1;
    uint8_t boatSpriteShift = 0;
    uint8_t QUACK = 5;

    uint8_t cursorYPos = 88; //start position of cursor. Not a const because that used precious data

    uint16_t demoTimer = 0;

    //Activate display
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    while(1){

        if(joypad()){
            demoTimer = 0;
        }

        if (joypad() & J_A && isPressed == 0){
            if (cursorYPos == 88){ //Fishin' Hole
                PREPARE_NEW_LEVEL;
                lvl_mainfishgame(0);
                gbt_stop();
                SR1;
                clear_OAM();
                hasnewhighscore(main_HighScores);
                lvl_HighScore(main_HighScores, 0);
                SR0;
                RESET_MENUSCREEN;
            }

            if (cursorYPos == 88+16){ //Subgames
                PREPARE_NEW_LEVEL;
                lvl_SubGames();
                RESET_MENUSCREEN;
            }
            if (cursorYPos == 88+32){ //HighScores
                PREPARE_NEW_LEVEL;
                SR1;
                set_win_tiles(0, 0, 14, 2, HUDCleaner); //clearing hud of pause menu garbage
                clear_OAM();
                lvl_HighScore(main_HighScores, 1);
                lvl_HighScore(hard_HighScores, 1);
                lvl_HighScore(basketball_HighScores, 1);
                lvl_HighScore(fishmode_HighScores, 1);
                set_win_tiles(0, 0, 14, 2, HUDMap); //re-adding pause menu garbage to hud
                SR0;
                RESET_MENUSCREEN;
            }

        }
        if (joypad() & J_SELECT && isPressed == 0 || demoTimer == 600){
                //isPressed = 1;
                PREPARE_NEW_LEVEL;
                SR1;
                set_hudmap_demo();
                SR0;
                lvl_demo();
                demoTimer = 0;
                gbt_stop();
                RESET_MENUSCREEN;
        }

        if (joypad() & J_START && isPressed == 0 && QUACK == 5){
            set_sprite_tile(1, 92);
            QUACK = 0;
            SR1;
            snd_quack();
            SR0;
            isPressed = 1;
        }

        if(!(joypad() & J_A) && !(joypad() & J_START)){
            isPressed = 0;
        }
        

        //Boat moves back and forth on bottom of screen
        if (boatPosX < 27 || boatPosX > 115){
            boatDir *= -1;

            if (boatDir == -1){ //if you're moving right, flip
                boatSpriteShift = 0;
                set_sprite_prop(1, 1 | get_sprite_prop(1) & ~S_FLIPX);
                set_sprite_prop(2, 1 | get_sprite_prop(2) & ~S_FLIPX);


            }
            else if (boatDir == 1){
                boatSpriteShift = 8;
                set_sprite_prop(1, 1 | S_FLIPX);
                set_sprite_prop(2, 1 | S_FLIPX);

            }
            MOVE_BOAT
            
        }
        if (boatDelayCounter == 3){
            boatPosX += boatDir;
            move_sprite(1, boatPosX + boatSpriteShift, 151);
            move_sprite(2, boatPosX + 8 - boatSpriteShift, 151);
            boatDelayCounter = 0;
        }
        boatDelayCounter++;
        demoTimer++;
        if (QUACK < 5){
            QUACK++;
        }
        if (QUACK == 5){
            set_sprite_tile(1, 79);
        }

        SR1;
        cursorYPos = menutraversal(88, 0, 2, 24, cursorYPos);
        performantdelay(delay_Time); //Some sort of delay is needed or music will shit itself
        SR0;
        //gbt_update(); //gbt player function. updates line of song we're playing
        //isPressed = 0;
    }

}