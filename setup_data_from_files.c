#include <c64.h>

char *const SCREEN_POINTER = (char *)$0400; // Pointer to screen data

const int SPRITE_ADDRESS = $2100;  
const int CHARSET_ADDRESS = $2000;
const int CHARSET_COLORS_ADDRESS = $3000;
const int MAP_ADDRESS = $3100;

const int NUMBER_OF_SPRITES = 3;

__address(SPRITE_ADDRESS) char SPRITES[] = kickasm(resource "data/Sprites.bin") {{
    .var sprite = LoadBinary("Sprites.bin")
    .fill sprite.getSize(), sprite.get(i)
}};

__address(CHARSET_ADDRESS) char CHARS[] = kickasm(resource "data/Chars.bin") {{
    .var chars = LoadBinary("Chars.bin")
    .fill chars.getSize(), chars.get(i)
}};

__address(CHARSET_COLORS_ADDRESS) char CHARS_COLORS[] = kickasm(resource "data/CharAttribs.bin") {{
    .var charsAttribs = LoadBinary("CharAttribs.bin")
    .fill charsAttribs.getSize(), charsAttribs.get(i)
}};

__address(MAP_ADDRESS) char MAP[] = kickasm(resource "data/Map.bin") {{
    .var map = LoadBinary("Map.bin")
    .fill map.getSize(), map.get(i)
}};


void main() {
  initialize();

  do {
    do {
    } while (VICII->RASTER != $ff);
    // animation
  } while (true);
}

void initialize() {
  VICII->MEMORY = toD018(SCREEN_POINTER, CHARS);
  VICII->SPRITES_ENABLE = %00000111;
  VICII->BORDER_COLOR = 0;
  VICII->BG_COLOR = 0;

  initScreenByMap(MAP);
  initSprites(NUMBER_OF_SPRITES);

  VICII->SPRITE0_X = 60;
  VICII->SPRITE0_Y = 80;
  VICII->SPRITE0_COLOR = $4;

  VICII->SPRITE1_X = 60;
  VICII->SPRITE1_Y = 120;
  VICII->SPRITE1_COLOR = $5;

  VICII->SPRITE2_X = 60;
  VICII->SPRITE2_Y = 160;
  VICII->SPRITE2_COLOR = $7;
}

void initSprites(int numOfSprites) {
  // addres where we have first pointer to sprite
  // last 8 bytes in screen memory
  char *spritesPointer = SCREEN_POINTER + $3f8; // 0x07F8

  // 1 block is 64 bytes
  char spriteBlockNumber = (char)((unsigned int)SPRITES / $40); // 0x0080

  // write block number on each sprite pointer
  // to show where are the data to the sprites
  for (int i = 0; i < numOfSprites; i++) {
    spritesPointer[i] = spriteBlockNumber++;
  }
}

void initScreenByMap(char* map) {
  for (int i = 0; i < 40*25; i++) {
    byte mapValue = map[i];
    
    SCREEN_POINTER[i] = mapValue;
    COLS[i] = CHARS_COLORS[mapValue];
  }
}
