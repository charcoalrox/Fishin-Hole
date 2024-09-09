#pragma bank 1

#include <gb/gb.h>
#include "../bgmdat/gbt_player.h"
#include <rand.h>
#include <string.h>

extern void snd_pauseOff(void);

extern UBYTE isPressed;


uint8_t menutraversal(uint8_t button1Pos, uint8_t cursorIDNO, uint8_t numOfButtons, uint8_t menuSelectorX, uint8_t menuSelectorY){
        //Assume cursor has been created and moved to initial position
        //Assume this function is being run in a while loop

        if(joypad() & J_UP && menuSelectorY != button1Pos){
            snd_pauseOff();
            menuSelectorY -= 16;
            move_sprite(cursorIDNO, menuSelectorX, menuSelectorY);
        }
        if(joypad() & J_DOWN && menuSelectorY != button1Pos + (numOfButtons * 16) ){
            snd_pauseOff();
            menuSelectorY += 16;
            move_sprite(cursorIDNO, menuSelectorX, menuSelectorY);
        }

        return menuSelectorY;
}


void performantdelay(uint8_t timeDelayed){
//primarily used to set game speed by lowering Gameboy's framerate
    uint8_t i;
    for(i = 0; i < timeDelayed; i++){
        wait_vbl_done();
    }     
}


void clear_OAM(void) {
//Delete all sprites saved in the OAM. Deals with visual bugs when screen changes happen
    uint8_t i;
    for(i = 0; i < 40; i++) {
        // Clear each sprite attribute entry
        set_sprite_prop(i, 0); // Clear sprite properties
        move_sprite(i, 0, 0);  // Move sprite off-screen (position 0,0)
    }
}


uint8_t randomNumberGenerator(uint8_t shiftTo, uint8_t endAt){
//return a random number in range: [startAt, endAt]
    uint16_t seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);

    return (((uint8_t)rand()) % endAt+1) + shiftTo;
}


void text_to_hud(char text[], unsigned char hud[], uint8_t x, uint8_t y, uint8_t w, uint8_t h){
//Take a string input and convert it to addresses of bg chars in system. Writes to a hud of your choice (OVERWRITING BACKGROUNDS. DO NOT USE HUDCLEANER HERE EVER)

    int8_t hexout;
    for (int i=0; i<20; i++){
        if (i<strlen(text)){
            hexout = (text[i] - '0') - 32; //takes position of string (text[i]), converts char to int (-'0'), converts int position to where it lives in gb RAM (-31)

            if (hexout == -48){ //handles spaces
                hud[i] = 0;
            }
            else{
                hud[i] = hexout;
            }
            }
            else{
                hud[i] = 0;
            }
        }
    set_win_tiles(x, y, w, h, hud);

    move_win(0, 0);
    SHOW_WIN;
}


void resetBackground(char background[]){
//Adds HUD elements back into background since demo modifies them out
    background[0] = 0x1c;
    background[1] = 0x15;
    background[2] = 0x26;
    background[3] = 0x15;
    background[4] = 0x1C;
    background[8] = 0x23;
    background[9] = 0x13;
    background[10] = 0x1F;
    background[11] = 0x22;
    background[12] = 0x15;
    background[16] = 0x24;
    background[17] = 0x19;
    background[18] = 0x1D;
    background[19] = 0x15;
}


void checkifPressed(uint8_t gamepadInput){
//Used with isPressed global to check if a button is held down. Mainly to stop inputs from registering before a level is loaded in the case of menus
    if (joypad() & gamepadInput){
        isPressed = 1;
    }
    else if (!(joypad() & gamepadInput)){
        isPressed = 0;
    }
}