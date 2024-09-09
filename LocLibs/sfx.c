#pragma bank 1

#ifndef SFX_C
#define SFX_C

#include <gb/gb.h>

//extern void performantdelay(uint8_t timeDelayed);

#define Stoplen 3 //sounds with more than one beep require a pause. Too long and it visibly stops the game. Adjusted to where I feel is right


void snd_dropnet(void){
    NR41_REG = 0x10;
    NR42_REG = 0xF2;
    NR43_REG = 0x49;
    NR44_REG = 0x80;
}


void snd_hitfish(void){
    NR41_REG = 0x34;
    NR42_REG = 0xF3;
    NR43_REG = 0x29;
    NR44_REG = 0xC0;
}


void snd_hitsub(void){
    NR41_REG = 0x3A;
    NR42_REG = 0xF1;
    NR43_REG = 0x61;
    NR44_REG = 0x80;
}

void snd_violent(void){
    NR41_REG = 0x39;
    NR42_REG = 0xF2;
    NR43_REG = 0x34;
    NR44_REG = 0x80;
}


void snd_UFO(void){
    NR41_REG = 0x32;
    NR42_REG = 0xF3;
    NR43_REG = 0x1F;
    NR44_REG = 0x80;
    //performantdelay(Stoplen);
    NR41_REG = 0x32;
    NR42_REG = 0xF3;
    NR43_REG = 0x08;
    NR44_REG = 0x80;
}


void snd_pauseOn(void){
    NR41_REG = 0x38;
    NR42_REG = 0xF3;
    NR43_REG = 0x28;
    NR44_REG = 0xC0;
    //performantdelay(Stoplen);
    NR41_REG = 0x38;
    NR42_REG = 0xF3;
    NR43_REG = 0x18;
    NR44_REG = 0xC0;
}


void snd_pauseOff(void){
    NR41_REG = 0x38;
    NR42_REG = 0xF1;
    NR43_REG = 0x38;
    NR44_REG = 0xC0;
}


void snd_LevelUp(void){
    NR41_REG = 0x39;
    NR42_REG = 0xF3;
    NR43_REG = 0x19;
    NR44_REG = 0xC0;
    //performantdelay(Stoplen);
    NR41_REG = 0x39;
    NR42_REG = 0xF3;
    NR43_REG = 0x19;
    NR44_REG = 0xC0;
}

void snd_quack(void){
    NR41_REG = 0x38;
    NR42_REG = 0x7C;
    NR43_REG = 0x3B;
    NR44_REG = 0xC0; 
}

#endif /* SFX_C */