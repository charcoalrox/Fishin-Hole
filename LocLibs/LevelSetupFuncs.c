///I had one for like every single level and I thought it would be nicer if they all went into 1 area
    ///Basically, I moved functions important for setting level globals into 1 function so I could push it into banking (or reset the map in the case of menus)

#pragma bank 1

#include <gb/gb.h> 
#include "../ImgCode/BackgroundData/background.h"
#include "../ImgCode/BackgroundData/BG_SubGames.h"
#include "../ImgCode/BackgroundData/BG_SubGames.c"
#include "../ImgCode/BackgroundData/BG_StartMenu.h" //Actual background file
#include "../ImgCode/HUDScreens/HUDMap.c"

#define MAX_SCORES 11
#define SCORE_LENGTH 9

extern uint8_t *main_HighScores[12];
extern uint8_t *hard_HighScores[12]; //hard mode scores
extern uint8_t *basketball_HighScores[12]; //basketball scores
extern uint8_t *fishmode_HighScores[12];

extern void movegamecharacter(struct GameCharacter* character, uint8_t x, uint8_t y, int8_t speed); //hack fix to hopefully stop collision bug?

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

extern const uint8_t numSprDict[10];

extern void setupgamecharacter(struct GameCharacter* person, uint8_t sprID, uint8_t sprite, uint8_t palID, uint8_t xLoc, uint8_t yLoc, uint8_t Xdir, uint8_t Ydir, uint8_t scoreValue, uint8_t currentSkin);
extern void objectflip(int8_t directionFromSpeed, struct GameCharacter* character, uint8_t x, uint8_t y);
extern void hudelementsinit(void);
extern void fishprofileinit(struct FishProfile* FishData, uint8_t sprID, uint8_t palID, uint8_t speed, uint8_t scoreVal);


void setupBasketballGame(void){
    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN0);

    //setting HUD elements
    hudelementsinit();

    //Initialize Objects
    setupgamecharacter(&man, 0, 0, 1, 9, 45, 2, 1, 99, 1);
    objectflip(-1, &man, man.xLoc, man.yLoc);
    set_sprite_tile(38, 76);

    set_sprite_tile(33, numSprDict[3]); //timer 10s place
}


void setupDemo(void){
    //Registers for sound to work
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    for (int i=0; i<20; i++){
        Background[i] = 0; //Removes HUD elements from background
    }

    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN0);
}


void setupFishMode(void){
    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN0);

    //setting HUD elements
    hudelementsinit();

    //Initialize Objects
    setupgamecharacter(&man, 0, 0, 1, 9, 45, 2, 1, 99, 1);
    objectflip(-1, &man, man.xLoc, man.yLoc);
    set_sprite_tile(5, 31); //Create net
    set_sprite_prop(5, 1);

    set_sprite_tile(34, numSprDict[9]); //timer 1s place
    set_sprite_tile(33, numSprDict[9]); //timer 10s place
    
}


void reloadSubGames(void){
    DISPLAY_OFF;
    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BG_SubGamesWidth, BG_SubGamesHeight, BG_SubGamesPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BG_SubGamesWidth, BG_SubGamesHeight, BG_SubGamesPLN0);   

    //Create cursor to interact with game
    set_sprite_tile(0, 29);
    set_sprite_prop(0, 3);
    move_sprite(0, 16, 48);
    DISPLAY_ON;
}


void reloadMainMenu(void){
//Recreate main menu screen and start positions when loading out of other screens

    DISPLAY_OFF;

    //Load proper background
    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BG_StartMenuWidth, BG_StartMenuHeight, BG_StartMenuPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BG_StartMenuWidth, BG_StartMenuHeight, BG_StartMenuPLN0);

    //Create cursor to interact with game
    set_sprite_tile(0, 29);
    set_sprite_prop(0, 3);

    //Set boat position, nb, and color
    set_sprite_tile(1, 79);
    set_sprite_tile(2, 80);

    move_sprite(1, 35, 151);
    set_sprite_prop(1, 1);
    move_sprite(2, 35+8, 151);
    set_sprite_prop(2, 1);

    //Cursor to default position (first button)
    move_sprite(0, 24, 88);

    //Enable sound registers. Redone every reload because stopping music breaks my registers
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    set_win_tiles(0, 0, 14, 2, HUDMap);
    move_win(7,120);

    DISPLAY_ON;
}


void set_hudmap_demo(void){
    //Sets the position of the hud to the demo screen (covers whole screen)
    set_win_tiles(0, 0, 14, 2, HUDMap);
}


void init_all_profiles(void){
    fishprofileinit(&snout, 5, 2, 3, 15);
    fishprofileinit(&pink, 9, 3, 7, 30);
    fishprofileinit(&puffer, 13, 2, 2, 15);
    fishprofileinit(&clown, 17, 4, 4, 10);
    fishprofileinit(&Angel, 21, 5, 5, 20);
}


void init_all_fish(uint8_t SelectedMode, UBYTE makeProfiles){
//Makes 4 fish. Proper setup for most games
    if (SelectedMode == 1){
        setupgamecharacter(&fish1, 5, 9, 3, 80, 120, 7, 1, 30, 9);
        setupgamecharacter(&fish2, 9, 9, 3, 96, 120, 7, 1, 30, 9);
        setupgamecharacter(&fish3, 13, 9, 3, 22, 130, 7, 1, 30, 9);
        setupgamecharacter(&fish4, 17, 9, 3, 38, 96, 7, 1, 30, 9);
    }
    else{ //I know it's weird for the default fish to go here but I want to reuse them in demo mode so this is the cleaner check
        setupgamecharacter(&fish1, 5, 5, 2, 80, 120, 3, 1, 15, 5);
        setupgamecharacter(&fish2, 9, 21, 5, 96, 120, 4, 1, 30, 21);
        setupgamecharacter(&fish3, 13, 13, 2, 22, 130, 3, 1, 15, 13);
        setupgamecharacter(&fish4, 17, 17, 4, 38, 96, 2, 1, 10, 17);
    }
    //Ideally this is gonna sync the fish with their locaiton on map startup and prevent the bug where collision only half works


    if (makeProfiles == 1){ //Hardmode and fishmode have slightly different setups that require init_all_profiles to be called separate sometimes
        init_all_profiles();
    }
}

void scoreInitializer(void){ //DefaultMode
//Allocates highscores to generic values when blank. Run on startup to ensure that user won't see junk variables before they have a save
    const uint8_t main_HS_Holder[MAX_SCORES][SCORE_LENGTH] = {
        {3, 17, 25, 0, 0, 5, 4, 2, 5},
        {1, 12, 7, 0, 0, 5, 0, 1, 9},
        {10, 22, 7, 0, 0, 4, 4, 4, 0},
        {7, 17, 6, 0, 0, 3, 5, 5, 9},
        {15, 19, 18, 0, 0, 2, 8, 9, 5},
        {9, 13, 17, 0, 0, 1, 8, 7, 0},
        {9, 9, 15, 0, 0, 1, 5, 0, 5},
        {1, 12, 17, 0, 0, 1, 0, 4, 5},
        {12, 3, 1, 0, 0, 0, 5, 0, 0},
        {18, 4, 10, 0, 0, 0, 0, 0, 0},
        {18, 4, 10, 0, 0, 0, 0, 0, 0}
    };

    const uint8_t hard_HS_Holder[MAX_SCORES][SCORE_LENGTH] = {
        {3, 17, 25, 0, 0, 4, 3, 1, 5},
        {3, 17, 25, 0, 0, 2, 4, 5, 0},
        {3, 17, 25, 0, 0, 1, 9, 5, 0},
        {3, 17, 25, 0, 0, 1, 8, 6, 0},
        {3, 17, 25, 0, 0, 1, 1, 9, 0},
        {3, 17, 25, 0, 0, 1, 1, 8, 5},
        {3, 17, 25, 0, 0, 1, 0, 8, 0},
        {3, 17, 25, 0, 0, 0, 9, 4, 5},
        {3, 17, 25, 0, 0, 0, 6, 7, 0},
        {18, 4, 10, 0, 0, 0, 0, 0, 0},
        {18, 4, 10, 0, 0, 0, 0, 0, 0}
    };

    const uint8_t basketball_HS_Holder[MAX_SCORES][SCORE_LENGTH] = {
        {3, 17, 25, 0, 0, 0, 0, 4, 4},
        {19, 14, 12, 0, 0, 0, 0, 3, 5},
        {2, 12, 17, 0, 0, 0, 0, 3, 3},
        {2, 17, 12, 0, 0, 0, 0, 3, 0},
        {3, 2, 1, 0, 0, 0, 0, 2, 6},
        {3, 0, 19, 0, 0, 0, 0, 2, 4},
        {12, 12, 15, 0, 0, 0, 0, 2, 0},
        {11, 9, 17, 0, 0, 0, 0, 1, 5},
        {10, 4, 3, 0, 0, 0, 0, 1, 2},
        {18, 4, 10, 0, 0, 0, 0, 0, 0},
        {18, 4, 10, 0, 0, 0, 0, 0, 0}
    };

    const uint8_t fish_HS_Holder[MAX_SCORES][SCORE_LENGTH] = {
        {3, 17, 25, 0, 0, 0, 0, 5, 9},
        {17, 6, 3, 0, 0, 0, 0, 5, 0},
        {9, 9, 2, 0, 0, 0, 0, 4, 4},
        {18, 14, 12, 0, 0, 0, 0, 4, 0},
        {4, 2, 3, 0, 0, 0, 0, 3, 4},
        {14, 24, 2, 0, 0, 0, 0, 2, 0},
        {3, 5, 2, 0, 0, 0, 0, 1, 9},
        {18, 18, 17, 0, 0, 0, 0, 1, 5},
        {15, 3, 3, 0, 0, 0, 0, 0, 3},
        {18, 4, 10, 0, 0, 0, 0, 0, 0},
        {18, 4, 10, 0, 0, 0, 0, 0, 0}
    };

    UBYTE savesUnallocated = 0; //Certain cartridges populate variables with 0'

    ENABLE_RAM_MBC1;
    if (main_HighScores[0][0]>0x28){
        savesUnallocated = 1;
    }
    else{
        for (int i=0; i<9; i++){
            if (!(main_HighScores[0][i] == 0)){
                savesUnallocated = 0;
            }
            else{
                savesUnallocated = 1;
            }
        }
    }

    if (savesUnallocated == 1){
        for (int i=0; i<MAX_SCORES; i++){
            for (int j=0; j<SCORE_LENGTH; j++){
                main_HighScores[i][j] = main_HS_Holder[i][j];
                hard_HighScores[i][j] = hard_HS_Holder[i][j];
                basketball_HighScores[i][j] = basketball_HS_Holder[i][j];
                fishmode_HighScores[i][j] = fish_HS_Holder[i][j];
            }
        }
    }
    DISABLE_RAM_MBC1;
}