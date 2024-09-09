#pragma bank 1

#include <gb/gb.h>

//Global variables used to keep track of user's score and scoremult
const uint8_t numSprDict[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41}; //array position is that number's position in RAM
uint8_t score[] = {0, 0, 0, 0, 0, 0};
uint8_t scoreMult = 0;

void hudelementsinit(void){
//Create hud elements on top of screen. All will be blank and set to 0 here
    const uint8_t yLevel = 25; //Everything is set to the same y-value. I think this is supposed to be good for ram

    //Create all letters in OAM. They're all set to 0 and colored black rn
    for (int i=25; i<35; i++){
        set_sprite_tile(i, 32);
        set_sprite_prop(i, 5);
    }

    //level num
    move_sprite(25, 16, yLevel);
    move_sprite(26, 24, yLevel);

    //score num
    move_sprite(27, 64, yLevel);
    move_sprite(28, 72, yLevel);
    move_sprite(29, 80, yLevel);
    move_sprite(30, 88, yLevel);
    move_sprite(31, 96, yLevel);
    move_sprite(32, 104, yLevel);

    //Timer
    move_sprite(33, 144, yLevel);
    move_sprite(34, 152, yLevel);
}


void clearHighscore(void){
//Since high score is a global variable, we have to make sure to reset it before loading any new games. This should be called every time a game is finished
    for (int i=0; i<sizeof(score); i++){ //since score is a global, we need to cleanse it every time we call a game
        score[i] = 0;
    }
}


void updatescore(uint8_t numAdded, uint8_t scoreMult){
    
    const uint8_t scoreMax = 5; //last position in score array

    if (scoreMult > 1){
        numAdded *= scoreMult;
    }

    score[scoreMax] += numAdded;
    uint8_t scorePos = 0;

    while(score[scoreMax-scorePos] > 0){
        if(score[scoreMax-scorePos] > 9){
            score[scoreMax - scorePos - 1] += score[scoreMax - scorePos]/10;
            score[scoreMax - scorePos] = score[scoreMax - scorePos]%10;
        }

        scorePos += 1;
        }

        //Now print these values onto the screen
        scorePos = 0;
        while(scorePos <= scoreMax){
            set_sprite_tile(32-scorePos, numSprDict[score[scoreMax - scorePos]]);
            scorePos += 1;
        }
}