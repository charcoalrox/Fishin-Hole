#include <gb/gb.h>
#include "../ImgCode/BackgroundData/background.h"
#include "../LocLibs/GameCharacter.h"
#include "../bgmdat/gbt_player.h"
#include "../LocLibs/unbankedfuncs.h"


#include <stdio.h>


extern void picksong(unsigned char songName[], uint8_t songSpeed);
extern const unsigned char * const bubble_machine_Data[];

extern void snd_dropnet(void);
extern void snd_hitsub(void);
extern void snd_LevelUp(void);
extern void snd_UFO(void);
extern void snd_hitfish(void);

extern void objectflip(int8_t directionFromSpeed, GameCharacter* character, uint8_t x, uint8_t y);
extern void movegamecharacter(GameCharacter* character, uint8_t x, uint8_t y, int8_t speed);
extern void setupgamecharacter(GameCharacter* person, uint8_t sprID, uint8_t sprite, uint8_t palID, uint8_t xLoc, uint8_t yLoc, uint8_t Xdir, uint8_t Ydir, uint8_t scoreValue, uint8_t currentSkin);
extern void performantdelay(uint8_t timeDelayed);
extern uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt);
extern void fishprofileinit(struct FishProfile* FishData, uint8_t sprID, uint8_t palID, uint8_t speed, uint8_t scoreVal);
extern UBYTE checkCollisions(uint8_t netLocX, uint8_t netLocY, uint8_t objLocX, uint8_t objLocY, uint8_t objWidth, uint8_t objHeight, int8_t handicap);
extern void fishBounce(struct GameCharacter* fish, uint8_t ySpeed, uint8_t yMin, uint8_t yMax, uint8_t pauseLen);
extern void fishpicker(struct GameCharacter* fish, struct FishProfile* fishSkin);
extern uint8_t fishBehavior(struct GameCharacter* fish, struct FishProfile* fishSKin);

extern void hudelementsinit(void);
extern void updatescore(uint8_t numAdded, uint8_t scoreMult);
extern const uint8_t numSprDict[10];

extern uint8_t movearrow(uint8_t arrowY);
extern uint8_t adjustarrow(uint8_t arrowX, uint8_t arrowY, uint8_t arrowYMiddleVal);
extern void player_thrownet(void);
extern void player_reset(void);
extern uint8_t pause_game(void);

extern void init_all_profiles(void);
extern void init_all_fish(uint8_t isHardMode, UBYTE makeProfiles);

extern UBYTE isPressed;


void lvl_mainfishgame(uint8_t SelectedMode){
    //Set proper background
    VBK_REG = 1; //switch to second memory bank (Sets colors. Always set this bank BEFORE set_bkg_tiles)
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN1); //Start at 0,0 and go to max size
    VBK_REG = 0;
    set_bkg_tiles(0, 0, BackgroundWidth, BackgroundHeight, BackgroundPLN0);

    SR1;
    //Create player and fish sprites
    setupgamecharacter(&man, 0, 0, 1, 9, 45, 2, 1, 99, 1); //Some of these values are gibberish because I recycled the fish code for man even though man really shouldn't be a global object if you think about it
    objectflip(-1, &man, man.xLoc, man.yLoc);

    init_all_fish(SelectedMode, 1);

    //Makes submarine
    set_sprite_tile(36, 69);
    set_sprite_prop(36, 7);
    set_sprite_tile(37, 70);
    set_sprite_prop(37, 7);

    //Create UFO at ideal height (y=32)
    set_sprite_tile(39, 30);
    set_sprite_prop(39, 5);
    move_sprite(39, 0, 0);

    //Create hud
    hudelementsinit();
    SR0;

    //Initialize arrow sprite
    uint8_t arrowX = 40;
    const uint8_t arrowYMiddleValue = 50;
    uint8_t arrowY = arrowYMiddleValue;
    set_sprite_tile(38, 76);
    set_sprite_prop(38, 3);
    
    move_sprite(38, arrowX, arrowY);
    SR0;

    //Initialize net sprite
    int8_t netVelMax = 15;
    int8_t netVelX = netVelMax;
    int8_t netVelY = 0;
    int8_t netInitPosition = 0; //Where net starts
    uint8_t netLocX = 0;
    uint8_t netLocY = 0;

    UBYTE netActive = 0; //Checks if net is already falling

    int8_t timerDelayer = 54; //must exist outside of timersetter func so that time can be constant and saved. Must be divisible by all versions of timerSpeed (set different for normal AND hard mode)
    uint8_t timerSpeed = 2;
    uint8_t timeRemaining = 10; //time put onto timer in HUD

    uint32_t scoreTotalInt = 0; //Total game score. Used in certain calcuations like leveling up
    uint8_t scoreMult = 0; //Number your score is multiplied by
    uint8_t level = 0; //Current level the game is on (decides difficulty and bragging rights)
    uint8_t levelKillCounter = 0; //Keeps track of kills. Used to reset timer at certain intervals
    
    //This UFO object is ALSO the 3 score multiplier objects. I named it UFO to avoid confusion with scoremult vairable
    UBYTE ufoisActive = 0;
    int8_t ufoSpeed = 3;
    uint8_t ufoScoreValue = 100;
    int8_t ufoX = 0;
    uint8_t ufoY = 40;
    uint8_t ufoMultiplier = 1; //This can be set in the initial switch case so that the collision code for the UFO can be a single code block

    int8_t submarineX = 120;
    uint8_t submarineY = 130;
    move_sprite(36, submarineX, submarineY);
    move_sprite(37, submarineX + 8, submarineY);

    uint8_t scoreStagger = 0; //For practice mode. Displays score for about 5 seconds before it resets to 0

    uint8_t maxLevelupQuota = 20; //Last level where your fish catching quota raises (fish you have to catch before the timer goes back to 0)

    if (SelectedMode == 1){ //Set meaner variables if you're a TRUE GAMER
        level = levelup(4);
        timerSpeed = 4;
        maxLevelupQuota = 25;
    }

    //Set song
    SR2;
    picksong(bubble_machine_Data, 1);
    SR0;

    isPressed = 1; //stops net from being thrown immediately on startup

    while(1){

        SR1;
        //Pause game
        if (joypad() & J_START && isPressed == 0){
            if (pause_game() == 1){
                isPressed = 1; //Stops game from going straight to demo screen or skipping initials input
                break;
            }
            isPressed = 1; //Stops holding down start from trapping player
        }
        SR0;

        //Score resets after a while in practice mode
        if (SelectedMode == 2){
            level = 1;
            scoreStagger ++;
            if (scoreStagger == 50){
                SR1;
                if (scoreMult == 0){ //practice mode shows you how much combos are worth. Doesn't save scores
                    clearHighscore(); //resets numbers on screen
                    scoreMult = 0;
                    updatescore(0, 0); //changes numbers on screen
                }
                SR0;

                scoreStagger = 0;
            }
        }

        if (SelectedMode != 2){ //Practice mode has no timer. Otherwise this should decrement
            //Decrement timer. End game if timer reaches 0
            timerDelayer-= timerSpeed;

            SR1;
            set_sprite_tile(33, numSprDict[timeRemaining/10]); //convert the 1's and 10's place from timerDelay into sprites on the screen
            set_sprite_tile(34, numSprDict[timeRemaining%10]);
            SR0;

            if (timerDelayer <= 0){ //When the counter delayer signals, the normal counter is decremented. If the game timer reaches 0, we kill it
                timeRemaining--;

                if (timeRemaining <= 0){
                    break;
                }
            }

            if (timerDelayer <= 0){ //Counting down once per frame is too fast. We put this here to delay that
                timerDelayer = 50;
            }

        //Increase game level if score//500 == 0. Reset timer. You earned it buddy
        if (scoreTotalInt >= (level)*500 || levelKillCounter == level || levelKillCounter == maxLevelupQuota){ //reset timer when you hit quota (quota=level. If level is more than 10 then quota = 10)
        //Timer resets if player catches the level+4 fish or levels up
            if (scoreTotalInt >= (level)*500){ //we only level up during this condition but all of them reset the timer
                level = levelup(level);
            }
            //This all resets the timer and the conditions to reset the timer as well
            SR1;
            set_sprite_tile(33, numSprDict[1]);
            set_sprite_tile(34, numSprDict[0]);
            SR0;
            levelKillCounter = 0;
            timeRemaining = 10;
        }

        }

        //Adjust player's arrow movement
        SR1;
        arrowY = movearrow(arrowY);
        arrowX = adjustarrow(arrowX, arrowY, arrowYMiddleValue);
        move_sprite(38, arrowX, arrowY);
        SR0;

        //Throw net if requested AND allowed
        if (joypad() & J_A){
            if (netActive == 0 && isPressed == 0){
                SR1;
                player_thrownet();
                SR0;

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

                set_sprite_tile(35, 31);
                set_sprite_prop(35, 1);
                netLocX = arrowX;
                move_sprite(35, netLocX, netLocY);

                netActive = 1;
            }
        isPressed = 1;
        }

        if(!(joypad() & J_A) && !(joypad() & J_START)){
            isPressed = 0;
        }

        //Calculate trajectory of the net
        if (netActive == 1){
            netLocY -= netVelY;
            if (netVelY > -6){
                netVelY--;
            }
            if (netVelX > 0){
                netLocX += netVelX;
                netVelX --;
            }
            move_sprite(35, netLocX, netLocY);
        }

        //Delete net upon collision with an object
        SR1;
        if ((netLocY > 160 || checkCollisions(netLocX, netLocY, submarineX, submarineY, 16, 8, 0)) && netActive == 1){
            netVelX = netVelMax;
            netActive = 0;
            scoreMult = 0;
            player_reset();
            netLocX = 0;
            netLocY = 0;
            move_sprite(35, netLocX, netLocY);
        }
        SR0;

        if (ufoX == 0){
            SR1;
            uint8_t UFOBehavior = randomNumberGenerator(0, 13);
            SR0;

            if (SelectedMode == 1 && UFOBehavior !=1){ //replace scoremult despawning, x2, and x3 with UFO encounter in hard mode
                UFOBehavior = 1;
            }

            if (UFOBehavior == 1){
            //This sets the UFO to an actual UFO
                ufoisActive = 1;
                set_sprite_tile(39, 30);
                ufoSpeed = 5;
                ufoY = 40;
                ufoMultiplier = 8; 
                ufoScoreValue = 100;
                if (SelectedMode != 1){ //This would get so annoying in hardmode where there are exclusively UFO's
                    SR1;
                    snd_UFO();
                    SR0;
                }
            }
            else if (UFOBehavior > 1 && UFOBehavior < 5){ 
            //Set UFO sprite to one of the scoremult OBJECTS (different from scoremult var)
                ufoisActive = 1; 
                ufoSpeed = 2; 
                ufoY = 88; 
                ufoScoreValue = 50;
                set_sprite_tile(39, (69 + UFOBehavior));
                ufoMultiplier = UFOBehavior - 1; //Because fish also add 1 to scoremult, we need to subtract 1 to set it equal to the sprite
            }
            else{
            //Otherwise, there is no scrolling score changer here
                ufoisActive = 0;
                ufoSpeed = 0;
            }
        }

        //ufoisActive = 1;
        if (ufoisActive == 1){ //relying on timerdelayer makes a fun rhythmic noise when we use remainders like this
            ufoX -= ufoSpeed; //If the lagging still persists, find out what this is when it reaches the wall and reset it to 0
            move_sprite(39, ufoX, ufoY);
        }

        SR1;
        if(checkCollisions(netLocX, netLocY, ufoX, ufoY, 8, 8, 6)){
            ufoisActive = 0;
            updatescore(ufoScoreValue, scoreMult);
            scoreTotalInt += ufoScoreValue;
            scoreMult += ufoMultiplier;
            ufoX = 0;
        }
        SR0;

        //Fish Collision and movement
        for (int i=0; i<4; i++){
            SR1;
            if (checkCollisions(netLocX, netLocY, fishList[i]->xLoc, fishList[i]->yLoc, fishList[i]->width, fishList[i]->height, 5) && (*fishList[i]).respawnTimer == 0 && netActive == 1){
                snd_hitfish();

                //fishpicker(&(*fishList[i]), skinList[randomNumberGenerator(0, 4)], SelectedMode);

                if (scoreMult < 9){
                    scoreMult += 1;
                }

                updatescore((*fishList[i]).scoreValue, scoreMult);
                scoreTotalInt += (*fishList[i]).scoreValue * scoreMult;
                levelKillCounter ++;

                (*fishList[i]).Xdir = 0;
                (*fishList[i]).Ydir = 0;
                (*fishList[i]).animDelayer = 1;
                (*fishList[i]).myScoreMult = scoreMult;
                (*fishList[i]).respawnTimer = 5; //How long between flashes on
            }
            //If the fish has fully stopped (should only happen if they've been hit by the net), then the death animation will play
            if ((*fishList[i]).Xdir == 0 && (*fishList[i]).xLoc != 0){
                uint8_t skinNum = randomNumberGenerator(0, 4);
                if (SelectedMode == 1){
                    skinNum = 1;
                }
                fishDeathAnim(&(*fishList[i]), skinList[skinNum]);
            }

            //Select behavior of fish from list using randomized number
            uint8_t skinNum2 = randomNumberGenerator(0, 4);
            if (SelectedMode == 1){
                skinNum2 = 1;
            }
            (*fishList[i]).selectMovement = fishBehavior(&(*fishList[i]), skinList[skinNum2]);
            if (SelectedMode != 1 && level < 10){ //Bouncing is for skilled GAMERS only. Must be hard mode or higher level
                (*fishList[i]).selectMovement = 1;
            }

            if ((*fishList[i]).selectMovement == 0){
                fishBounce(&(*fishList[i]), 5, 96, 144, 3);
            }
            else if ((*fishList[i]).Xdir != 0){ //mostly for select movement == 1, Else check in case something goes horribly wrong
                movegamecharacter(&(*fishList[i]), (*fishList[i]).xLoc, (*fishList[i]).yLoc, (*fishList[i]).Xdir);
            }
            SR0;
        }

        //Submarine movement. Moves left constantly. Y randomized at top
        if (level < 6){
            submarineX -= level;
        }
        else { 
            submarineX -= 8; //once submarine gets fast enough it's not an obstacle anymore because you just can't realy hit it. Necessary cap
        }

        uint8_t submarineYOffset = 75;
        if (level == 10){ //Raises submarine more towards the top of the water. Really annoying. Only when people get the MOST punished for playing this crap too long
            submarineYOffset = 60;
        }

        move_sprite(36, submarineX, submarineY);
        move_sprite(37, submarineX + 8, submarineY);

        if (submarineX+8 == 0){
            SR1;
            uint8_t subRandY = randomNumberGenerator(0, 82);
            SR0;
            submarineY = subRandY + submarineYOffset;
        }

        //Misc enginge functions
        SR1;
        performantdelay(3);
        gbt_update();
        gbt_update();
        gbt_update();
        SR0;
    }
}