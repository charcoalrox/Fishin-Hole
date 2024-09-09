#pragma bank 1

#include "gb/gb.h"
#include "string.h" //string copy. Important for high score labels
#include "../ImgCode/BackgroundData/BG_HighScores.h"
#include "../bgmdat/gbt_player.h"

#define MAP_WIDTH 20
#define MAP_HEIGHT 18
#define BG_NewHighScore BG_NewHighScorePLN0

extern UBYTE isPressed;

//From Hud Elements. Tracks player's score. I don't think scoremult is necessary but it's here while I make some modifications to other parts of code
extern uint8_t score[6];
extern uint8_t scoreMult;

//Converters. These take real numbers and move them to the proper place in memory for the sprites. They're like pointers but stupider
extern uint8_t numSprDict[10]; //array position is that number's position in RAM

uint8_t map_NumSprDict[11] = {103, 104, 105, 106, 107, 108, 109, 110, 111, 112}; //array position is that number's position in RAM
uint8_t map_CharSprDict[27] = {17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42}; //array position is the num's position in the alphabet minus 1

//Global variables to describe the arrays containing background data
extern unsigned char BlueFlashMap[34];

extern unsigned char BG_NewHighScorePLN0[];
extern unsigned char BG_NewHighScorePLN1[];

extern unsigned char MapTileSet[];
extern unsigned char GameSprites[];

extern void snd_pauseOff(void);
extern void snd_hitfish(void);

extern void clear_OAM(void);
extern void text_to_hud(char text[], unsigned char hud[], uint8_t x, uint8_t y, uint8_t w, uint8_t h);
extern void performantdelay(uint8_t timeDelayed);




void setup_Highscore_MainStart(void){

    //Registers for sound to work
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    //Initialize sprites
    set_sprite_data(0, 73, GameSprites);
    set_bkg_data(0, 113, MapTileSet); 

    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BG_HighScoresWidth, BG_HighScoresHeight, BG_HighScoresPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BG_HighScoresWidth, BG_HighScoresHeight, BG_HighScoresPLN0);

}


void updateHighScorePage(unsigned char BGArray[], uint8_t *TwoD_Array[]){
    //Update the background on a high score menu to display appropriate scores. Starts at first letter in initials on first line. Ends on last digit of last high score
    uint8_t scoreArrayPos = 0; //represents the 'x' value of main_HighScores for the j loop

    if (TwoD_Array[10][0] == 27){ //27 is out of range. This prints to you if you have an invalid number in the initials (basically checks if you got a high score)
        TwoD_Array[10][0] = 24;
        TwoD_Array[10][1] = 14;
        TwoD_Array[10][2] = 20;
    }
    for (int i=3; i<9; i++){ //Update the player's high score to be the global final score they got minus the initials. This way their score displays on the bottom if they didn't get a high score
        TwoD_Array[10][i] = score[i-3];
    }

    for (int linepos=64; linepos<304; linepos+=10){

        if (linepos == 264){ //blank line between 10th high score and player's score gets skipped
            linepos += 20;
        }
        for (int j=0; j<9; j++){
            if (j<3){
                BGArray[linepos] = map_CharSprDict[TwoD_Array[scoreArrayPos][j]];
            }
            else if (j == 3){
                //BG_HighScores[linepos] = 0;
                linepos++;
                BGArray[linepos] = map_NumSprDict[TwoD_Array[scoreArrayPos][j]];
                //k++;
            }
            else if (j>3){
                BGArray[linepos] = map_NumSprDict[TwoD_Array[scoreArrayPos][j]];
            }

            linepos++;
        }
        scoreArrayPos++;
    }
    
}


void setup_NewHighscore(void){
    //Registers for sound to work
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    //Initialize sprites
    set_sprite_data(0, 83, GameSprites);
    set_bkg_data(0, 114, MapTileSet); 

    //Set the 3 initials as sprite objects
    for (int i=0; i<3; i++){
        set_sprite_tile(i+2, 42);
        set_sprite_prop(i+2, 5);
        move_sprite(i+2, 48+(i*8), 63);
    }

    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, MAP_WIDTH, MAP_HEIGHT, BG_NewHighScorePLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, MAP_WIDTH, MAP_HEIGHT, BG_NewHighScorePLN0);

}


int initialsincrement(uint8_t initialPicker, int8_t modifiedInitial, int8_t alphabetIncrementDirection){
//position in array, output number we're changing to, increment direction
//Move the letters on the enter initials screen of the high score function

    modifiedInitial+= alphabetIncrementDirection; //This way the number can be positive OR negative 1 to change the direction of letters

    //Loop letters to other end of the alphabet if user tries to scroll past A or Z
    if (modifiedInitial > 67){
        modifiedInitial = 42;
    }
    if (modifiedInitial < 42){
        modifiedInitial = 67;
    }

    set_sprite_tile(2+initialPicker, modifiedInitial); //update the appropriate sprite tile to their new number
    return modifiedInitial;
}


void highscoreupdater(uint8_t *TwoD_Array[]){
//Checks if the player got a new high score, updates the charts accordingly
//Currently only built to work with main_HighScores. Could easily be called for all minigames if you swap that with a blank var for the func call

    uint8_t scoreHolder[9]; //Holds the score the loop is currently erasing so we can move it down to the next wrung
    uint8_t scoreHolder2[9]; //Holds the score below what is currently being erased so scoreHolder 1 can overwrite it
    UBYTE firstLoopDone = 0; //Checks if the loop containing the player's high score has been erased
    uint8_t newScoreLine = 11; //Important for math that decides what the last line the player can overwrite is
    for (int i=0; i<10; i++){ //i is the current line of the high score board we're incrementing down
    
        for (int j=3; j<9; j++){ //J is the char position of the ith line

            if (TwoD_Array[i][j] > TwoD_Array[10][j]){
                break; //The score at this wrung of the High Score board is higher. Move down to the next one
            }

            if (TwoD_Array[i][j] < TwoD_Array[10][j]){
                for (int k=0; k<9; k++){ //If player's score is found to be larger than the ith score, it's overwritten

                    if (firstLoopDone == 0){ //scores can't be double digits for a single postion so I'm essentially checking if scoreholder's last potision is a NULL value
                        scoreHolder[k] = TwoD_Array[i][k];
                        //scoreHolder2[k] = scoreHolder[k];
                        TwoD_Array[i][k] = TwoD_Array[10][k]; //Player high score higher. Overwrite this one with theirs
                        scoreHolder2[k] = TwoD_Array[i+1][k];
                        if (i+1 != 10){
                            TwoD_Array[i+1][k] = scoreHolder[k];
                        }
                    }
                    if (firstLoopDone == 1){
                        scoreHolder[k] = scoreHolder2[k];
                        if (i != 9){ //try this with 10 and 8 maybe?
                            scoreHolder2[k] = TwoD_Array[i+1][k];
                        }
                        if (i+1 != 10){
                            TwoD_Array[i+1][k] = scoreHolder[k];
                        }
                    }
                    if (k == 8){
                        firstLoopDone = 1;
                    }

                }
                break;
            }
        }
    }
}


void lvl_HighScore(uint8_t *TwoD_Array[], UBYTE fromMain){
    DISPLAY_OFF;
    ENABLE_RAM_MBC1; //High score becomes too yummy if this is disabled. DO NOT
    highscoreupdater(TwoD_Array);

    isPressed = 1;//Going from game or menu directly here won't let you skip immediately

    clear_OAM();
    updateHighScorePage(BG_HighScores, TwoD_Array);
    //Setup_highscore freezes screen. Informational screens must be set up first

    if (fromMain == 1){
    //This will run if you're running high score button on main menu. Not when you're scoring in the game

        char text[] = "fishin hole";

        uint8_t value = &TwoD_Array[11][0]; //Grab array's id number to decide the title

        switch (value)
        {
        case 0:
            strcpy(text, "fishin hole");
            break;
        case 1:
            strcpy(text, "hard mode");
            break;
        case 2:
            strcpy(text, "basketball");
            break;
        case 3:
            strcpy(text, "fish mode");
            break;
        default:
            strcpy(text, "you broke it");
            break;
        }
        text_to_hud(text, BlueFlashMap, 0, 0, 15, 1); //I'm using blueflashmap over hudmap because it's blank by default and overwrites itself before use
        move_win(7, 104); //Just high enough to cover user's score. Convenient...
        SHOW_WIN;
    }

    setup_Highscore_MainStart();

    DISPLAY_ON;

    while(1){
        if (joypad() & J_A && isPressed == 0){ //reset high score variables so it's a clean slate every time
            snd_hitfish();
            ENABLE_RAM_MBC1;
            TwoD_Array[10][0] = 24;
            TwoD_Array[10][1] = 14;
            TwoD_Array[10][2] = 20;
            for (int i=3; i<9; i++){ //Update the player's high score to be the global final score they got minus the initials. This way their score displays on the bottom if they didn't get a high score
                TwoD_Array[10][i] = 0;
            }
            DISABLE_RAM_MBC1;
            HIDE_WIN;
            isPressed = 1;
            break;

        }

        if(!(joypad() & J_A)){
            isPressed = 0;
        }
    }
    DISABLE_RAM_MBC1;
}


void lvl_NewHighScore(uint8_t *TwoD_Array[]){
    DISPLAY_OFF;
    
    for (int i = 0; i<6; i++){
        BG_NewHighScore[129 + i] = map_NumSprDict[score[i]];
    }

    setup_NewHighscore();
    SHOW_SPRITES;
    SHOW_BKG;

    isPressed = 1; //Going from game or menu directly here won't let you skip immediately

    set_sprite_tile(0, 81);
    set_sprite_prop(0, 3);
    set_sprite_tile(1, 82);
    set_sprite_prop(1, 3);
    uint8_t cursorPosX = 48;
    uint8_t cursorPosY = 54;
    uint8_t initialPicker = 0;
    uint8_t initial1 = 42;
    uint8_t initial2 = 42;
    uint8_t initial3 = 42;

    uint8_t initialArray[] = {initial1, initial2, initial3};

   BG_NewHighScore[129] = map_NumSprDict[score[0]];

    DISPLAY_ON;

    while (1){
        if (joypad() & J_LEFT && cursorPosX > 48){
            snd_pauseOff();
            cursorPosX -= 8;
            initialPicker --;
        }
        if (joypad() & J_RIGHT && cursorPosX < 64){
            snd_pauseOff();
            cursorPosX += 8;
            initialPicker ++;
        }
        move_sprite(0, cursorPosX, cursorPosY);
        move_sprite(1, cursorPosX, cursorPosY+18);

        if (joypad() & J_DOWN){
            initialArray[initialPicker] = initialsincrement(initialPicker, initialArray[initialPicker], 1);
        }
        if (joypad() & J_UP){
            initialArray[initialPicker] = initialsincrement(initialPicker, initialArray[initialPicker], -1);
        }

        if (joypad() & J_A && isPressed == 0){
            snd_hitfish();
            ENABLE_RAM_MBC1;
            TwoD_Array[10][0] = initialArray[0]-42; //-42 because I'm converting the object sprite values to be able to run through the map text converter. It's a bit messy but that's what no pre-planning gets
            TwoD_Array[10][1] = initialArray[1]-42;
            TwoD_Array[10][2] = initialArray[2]-42;
            for (int i = 3; i<9; i++){
                TwoD_Array[10][i] = score[i-3];
            }
            DISABLE_RAM_MBC1;
            //isPressed = 1;
            break;
        }

        if(!(joypad() & J_A)){
            isPressed = 0;
        }

        performantdelay(6);
    }
}


int hasnewhighscore(uint8_t *TwoD_Array[]){
//Checks if the player got a new high score. For use of checking if we should open the enter initials page
//This is just the highscoreupdater() function modified and I should have used that one function for both but I've been writing a lot today so sorry. Maybe I'll fix this later
    ENABLE_RAM_MBC1;

    for (int i=0; i<10; i++){ //i is the current line of the high score board we're incrementing down
    
        for (int j=3; j<9; j++){ //J is the char position of the ith line

            if (TwoD_Array[i][j] > score[j-3]){
                break; //It's assumed the high score board will be called under this function so we're not calling it here
            }

            if (TwoD_Array[i][j] < score[j-3]){
                lvl_NewHighScore(TwoD_Array);
                DISABLE_RAM_MBC1;
                return 0;
            }
        }
    }
    DISABLE_RAM_MBC1;
    return 1;
}