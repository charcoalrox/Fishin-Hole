#include <gb/gb.h>
#include "FishMode.c"

extern void lvl_mainfishgame(uint8_t SelectedMode);

#define TOP_BUTTON 48

//Slightl different from the main ones since I'm loading subgames. Maybe these could have all been a function but I'm a busy boy
#define PREP_NEW_LEVEL SR1; snd_hitfish(); clear_OAM(); clearHighscore(); SR0;
#define RELOAD_SUBGAMES SR1; clear_OAM(); reloadSubGames(); SR0; cursorYPos = TOP_BUTTON; break;


//extern uint8_t *main_HighScores[12];
extern uint8_t *hard_HighScores[12]; //hard mode scores
extern uint8_t *basketball_HighScores[12]; //basketball scores
extern uint8_t *fishmode_HighScores[12];

//Banked Functions
extern void picksong(unsigned char songName[], uint8_t songSpeed);
extern uint8_t menutraversal(uint8_t button1Pos, uint8_t cursorIDNO, uint8_t numOfButtons, uint8_t menuSelectorX, uint8_t menuSelectorY);
extern void performantdelay(uint8_t timeDelayed);
extern void clear_OAM(void);

extern void clearHighscore(void);

extern void snd_hitfish(void);

extern const uint8_t delay_Time;

extern void reloadSubGames(void);

extern int hasnewhighscore(uint8_t *TwoD_Array[]);
extern void lvl_HighScore(uint8_t *TwoD_Array[], UBYTE fromMain);

extern void lvl_basketball(void);

extern UBYTE isPressed;

void lvl_SubGames(void){
    SR1;
    reloadSubGames();
    SR0;
    uint8_t cursorYPos = 48; //start position of cursor. Not a const because that used precious data
    isPressed = 1;

    while (1){
        SR1;
        cursorYPos = menutraversal(48, 0, 3, 16, cursorYPos);
        performantdelay(delay_Time); //Some sort of delay is needed or music will shit itself
        SR0;

        if (joypad() & J_A && isPressed == 0){
            switch (cursorYPos)
            {
            case TOP_BUTTON: //Practice Mode
                PREP_NEW_LEVEL;
                lvl_mainfishgame(2);
                gbt_stop();

                RELOAD_SUBGAMES;
            case TOP_BUTTON + 16: //Hard Mode
                PREP_NEW_LEVEL;
                lvl_mainfishgame(1);
                gbt_stop();
                SR1;
                clear_OAM();
                hasnewhighscore(hard_HighScores);
                lvl_HighScore(hard_HighScores, 0);
                SR0;

                RELOAD_SUBGAMES;
            case TOP_BUTTON + 32: //Basketball
                PREP_NEW_LEVEL;
                SR1;
                lvl_basketball();
                gbt_stop();

                clear_OAM();
                hasnewhighscore(basketball_HighScores);
                lvl_HighScore(basketball_HighScores, 0);
                SR0;

                RELOAD_SUBGAMES;
            case TOP_BUTTON + 48: //Fish Mode
                PREP_NEW_LEVEL;
                lvl_fishmode();
                gbt_stop();

                SR1;
                clear_OAM();
                hasnewhighscore(fishmode_HighScores);
                lvl_HighScore(fishmode_HighScores, 0);
                SR0;

                RELOAD_SUBGAMES;

            default:
                break;
            }
        }

        if (!(joypad() & J_A)){ //Stops buttons from being selected by naughty players holding the button down
            isPressed = 0;
        }

    if (joypad() & J_B){
        break;
    }
    }
}