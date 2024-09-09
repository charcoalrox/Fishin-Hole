#pragma bank 1

#include <gb/gb.h>

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

extern void snd_hitsub(void);
extern void snd_dropnet(void);


uint8_t movearrow(uint8_t arrowY){
//Move arrow up or down
    if (joypad() & J_UP && arrowY>=man.yLoc-8){
        arrowY-=4;
    }
    if (joypad() & J_DOWN && arrowY<=man.yLoc+21){
        arrowY+=4;
    }
    return arrowY;
}


uint8_t adjustarrow(uint8_t arrowX, uint8_t arrowY, uint8_t arrowYMiddleVal){
//Adjust arrow x value and sprite based on arrow y value. Assumed you're running directly after movearrow in a loop
    if(joypad() & J_UP && arrowY>=man.yLoc-8){
        if (arrowY < arrowYMiddleVal){
            arrowX--;
        }
        if (arrowY > arrowYMiddleVal){
            arrowX++;
        }
        if (arrowY == arrowYMiddleVal){
            arrowX = 40;
        }
    }
    if(joypad() & J_DOWN && arrowY<=man.yLoc+21){
        if (arrowY > arrowYMiddleVal){
            arrowX--;
        }
        if (arrowY < arrowYMiddleVal){
            arrowX++;
        }
        if (arrowY == arrowYMiddleVal){
            arrowX = 40;
        }
    }
    
    //Sets where arrow points based on it's y coordinate
    switch (arrowY)
    {
    case 42:
        set_sprite_tile(38, 75);
        break;
    case 50: //imagine these are arithmetic on arrowYMiddleValue. The system is LYING and saying it's not a const!
        set_sprite_tile(38, 76);
        break;
    case 54:
        set_sprite_tile(38, 77);
        break;
        
    default: //do nothing
        break; //to denote failure. I'm hoping this doesn't break anything because the game won't compile if I refuse to return
    }

    return arrowX;
}


void player_thrownet(void){
//Small animation for player throwing net. Reused enough to need a func
    set_sprite_tile(0, 25);
    set_sprite_tile(1, 26);
    set_sprite_tile(2, 27);
    set_sprite_tile(3, 28);
    snd_dropnet();
}


void player_reset(void){
//Reset player to default animation when the net collides with something
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    //snd_hitsub();
}