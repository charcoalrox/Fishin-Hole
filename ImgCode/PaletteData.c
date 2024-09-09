#include <gb/gb.h> //GBDK libs
#include <gb/cgb.h> //includes gbc functionality (for colors)
#include "GameSprites.h" //Sprite tilesheet as code
#include "MapTileSet.h" //Tileset for background
#include "BackgroundData/Background.h" //data for background (very short-sighted name. I know)

const UWORD backgroundpalette[] = {
/* Gameboy Color palette 0 */
MapTileSetCGBPal0c0,
MapTileSetCGBPal0c1,
MapTileSetCGBPal0c2,
MapTileSetCGBPal0c3,

/* Gameboy Color palette 1 */
MapTileSetCGBPal1c0,
MapTileSetCGBPal1c1,
MapTileSetCGBPal1c2,
MapTileSetCGBPal1c3,

/* Gameboy Color palette 2 */
MapTileSetCGBPal2c0,
MapTileSetCGBPal2c1,
MapTileSetCGBPal2c2,
MapTileSetCGBPal2c3,

/* Gameboy Color palette 3 */
MapTileSetCGBPal3c0,
MapTileSetCGBPal3c1,
MapTileSetCGBPal3c2,
MapTileSetCGBPal3c3,

/* Gameboy Color palette 4 */
MapTileSetCGBPal4c0,
MapTileSetCGBPal4c1,
MapTileSetCGBPal4c2,
MapTileSetCGBPal4c3,

/* Gameboy Color palette 5 */
MapTileSetCGBPal5c0,
MapTileSetCGBPal5c1,
MapTileSetCGBPal5c2,
MapTileSetCGBPal5c3,

/* Gameboy Color palette 6 */
MapTileSetCGBPal6c0,
MapTileSetCGBPal6c1,
MapTileSetCGBPal6c2,
MapTileSetCGBPal6c3,

/* Gameboy Color palette 7 */
MapTileSetCGBPal7c0,
MapTileSetCGBPal7c1,
MapTileSetCGBPal7c2,
MapTileSetCGBPal7c3,
};

const UWORD spritepalette[] = {
/* Gameboy Color palette 0 */
GameSpritesCGBPal0c0,
GameSpritesCGBPal0c1,
GameSpritesCGBPal0c2,
GameSpritesCGBPal0c3,

/* Gameboy Color palette 1 */
GameSpritesCGBPal1c0,
GameSpritesCGBPal1c1,
GameSpritesCGBPal1c2,
GameSpritesCGBPal1c3,

/* Gameboy Color palette 2 */
GameSpritesCGBPal2c0,
GameSpritesCGBPal2c1,
GameSpritesCGBPal2c2,
GameSpritesCGBPal2c3,

/* Gameboy Color palette 3 */
GameSpritesCGBPal3c0,
GameSpritesCGBPal3c1,
GameSpritesCGBPal3c2,
GameSpritesCGBPal3c3,

/* Gameboy Color palette 4 */
GameSpritesCGBPal4c0,
GameSpritesCGBPal4c1,
GameSpritesCGBPal4c2,
GameSpritesCGBPal4c3,

/* Gameboy Color palette 5 */
GameSpritesCGBPal5c0,
GameSpritesCGBPal5c1,
GameSpritesCGBPal5c2,
GameSpritesCGBPal5c3,

/* Gameboy Color palette 6 */
GameSpritesCGBPal6c0,
GameSpritesCGBPal6c1,
GameSpritesCGBPal6c2,
GameSpritesCGBPal6c3,

/* Gameboy Color palette 7 */
GameSpritesCGBPal7c0,
GameSpritesCGBPal7c1,
GameSpritesCGBPal7c2,
GameSpritesCGBPal7c3,
};