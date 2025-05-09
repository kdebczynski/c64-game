#include <c64.h>

char *const SCREEN_POINTER = (char *)$0400;       // Pointer to screen data
char *const SPRITE_POINTER_DATA = (char *)$2800;  // Pointer to sprite data
char *const CHARSET_POITER = (char *)$2000;

const int MAX_CHARS = 4;

void main() {
  initialize();

  do {
    do {
    } while (VICII->RASTER != $ff);
    // animation
  } while (true);
}

void initialize() {
  VICII->MEMORY = toD018(SCREEN_POINTER, CHARSET_POITER);
  VICII->SPRITES_ENABLE = %00000111;

  clearScreen();
  initChars();
  initSpritesMemory(3);
  writeSpritesData();

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

void initSpritesMemory(int numOfSprites) {
  // addres where we have first pointer to sprite
  // last 8 bytes in screen memory
  char *spritesPointer = SCREEN_POINTER + $3f8; // 0x07F8

  // 1 block is 64 bytes
  char spriteBlockNumber = (char)((unsigned int)SPRITE_POINTER_DATA / $40); // 0x0080

  // write block number on each sprite pointer
  // to show where are the data to the sprites
  for (int i = 0; i < numOfSprites; i++) {
    spritesPointer[i] = spriteBlockNumber++;
  }
}

void writeSpritesData() {
  char *spr = SPRITE_POINTER_DATA;

  writeSpriteData(spr, SPRITE_1);

  spr = spr + $40;
  writeSpriteData(spr, SPRITE_2);

  spr = spr + $40;
  writeSpriteData(spr, SPRITE_3);
}

void writeSpriteData(char* spr, char* spriteData) {
  for (int i = 0; i < 64; i++) {
      spr[i] = spriteData[i];
  }
}

void clearScreen() {
  for (char *sc = SCREEN_POINTER; sc < SCREEN_POINTER + 40*25; sc++) {
    *sc = 0;
  }
}

void initChars() {
  for (int i = 0; i < MAX_CHARS*8; i++) {
    CHARSET_POITER[i] = tiles[i];
  }
}

char tiles[MAX_CHARS*8] = {
  %00000000,
  %00000000,
  %00000000,
  %00011000,
  %00011000,
  %00000000,
  %00000000,
  %00000000,

  %00000001,
  %00000001,
  %00000001,
  %00000001,
  %00000001,
  %00000001,
  %00000001,
  %00000001,

  %10000000,
  %10000000,
  %10000000,
  %10000000,
  %10000000,
  %10000000,
  %10000000,
  %10000000,

  %10000001,
  %10000001,
  %10000001,
  %10000001,
  %10000001,
  %10000001,
  %10000001,
  %10000001,
};

char SPRITE_1[64] = kickasm(resource "data/Sprites.bin") {{
    .var sprite = LoadBinary("Sprites.bin")
    .fill sprite.getSize(), sprite.get(i)
}};

char SPRITE_2[64] = kickasm(resource "data/balloon.png") {{
    .var pic = LoadPicture("balloon.png", List().add($000000, $ffffff))
    .for (var y=0; y<21; y++)
        .for (var x=0;x<3; x++)
            .byte pic.getSinglecolorByte(x,y)
}};

char SPRITE_3[64] = {
  0b00000000, 0b00011000, 0b00000000,
  0b00000000, 0b00111100, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b01111110, 0b00000000,
  0b00000000, 0b11111111, 0b00000000,
  0b00000000, 0b11111111, 0b00000000,
  0b00000000, 0b11111111, 0b00000000,
  0b00000001, 0b11111111, 0b10000000,
  0b00000011, 0b11111111, 0b11000000,
  0b00000111, 0b11111111, 0b11100000,
  0b00001111, 0b11111111, 0b11110000,
  0b00011000, 0b11111111, 0b00011000,
  0b00010000, 0b01111110, 0b00001000,
  0b00000000, 0b00111100, 0b00000000,
  0b00000000, 0b00011000, 0b00000000
};
