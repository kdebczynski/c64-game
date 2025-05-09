#include <c64.h>

char *const SCREEN_POINTER = (char *)$0400;       // Pointer to screen data
char *const SPRITE_POINTER_DATA = (char *)$2800;  // Pointer to sprite data
char *const CHARSET_POINTER_DATA = (char *)$2000; // Pointer to chars data

__address($2800) char SPRITES[] = kickasm(resource "data/Sprites.bin") {{
    .var sprite = LoadBinary("Sprites.bin")
    .fill sprite.getSize(), sprite.get(i)
}};

__address($2000) char CHARS[] = kickasm(resource "data/Chars.bin") {{
    .var chars = LoadBinary("Chars.bin")
    .fill chars.getSize(), chars.get(i)
}};

const int NUMBER_OF_SPRITES = 3;

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

  clearScreen();
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

void clearScreen() {
  for (char *sc = SCREEN_POINTER; sc < SCREEN_POINTER + 40*25; sc++) {
    *sc = 1;
  }
}
