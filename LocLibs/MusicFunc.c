#pragma bank 2
//Yes really a whole page for 1 function. It's in the music bank because it needs to reference bank 2 stuff


#include<gb/gb.h>
#include "../bgmdat/gbt_player.h"

void picksong(unsigned char songName[], uint8_t songSpeed){ //Eventually I think songspeed can just be 7 and not a variable. I wanna see how this interacts with my other games first tho
//OpenMPT song switcher workflow. Much faster and cleaner than retyping everything every time

    disable_interrupts();

    gbt_play(songName, 2, songSpeed);
    gbt_loop(1);

    set_interrupts(VBL_IFLAG);
    enable_interrupts();
}