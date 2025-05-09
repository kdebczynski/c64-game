#include <c64.h>

char *const SCREEN_POINTER = (char *)$0400; // Pointer to screen data

const int SPRITE_ADDRESS = $2100;  
const int CHARSET_ADDRESS = $2000;
const int CHARSET_COLORS_ADDRESS = $3000;
const int MAP_ADDRESS = $3100;

const int SPRITE_FRAME_DELAY = 13;
int frameCounter = 0;

const char SPRITES_FRAMES_COUNT[] = {6, 2, 2}; 
char spritesCurrentFrame[] = {0, 0, 0};

void main() {
  initialize();

  do {
    wait_vblank();  // synchronizacja z ramką

    frameCounter++;

    if (frameCounter >= SPRITE_FRAME_DELAY) {
        frameCounter = 0;

        spritesCurrentFrame[0] = getNextSpriteFrameBySpriteNumber(0);
        spritesCurrentFrame[1] = getNextSpriteFrameBySpriteNumber(1);
        spritesCurrentFrame[2] = getNextSpriteFrameBySpriteNumber(2);

        // TODO: calculate data position by sprite number and frames count
        updateSpriteFrame(0, spritesCurrentFrame[0], 0);
        updateSpriteFrame(1, spritesCurrentFrame[1], 2);
        updateSpriteFrame(2, spritesCurrentFrame[2], 4);
    }
  } while (true);
}

void initialize() {
  VICII->MEMORY = toD018(SCREEN_POINTER, CHARS);
  VICII->SPRITES_ENABLE = %00000111;
  VICII->SPRITES_MC = %00000111;
  VICII->BORDER_COLOR = 0;
  VICII->BG_COLOR = 0;

  VICII->SPRITES_MCOLOR1 = 3;
  VICII->SPRITES_MCOLOR2 = 6;

  initScreenByMap(MAP);
  initSprites();

  VICII->SPRITE0_X = 60;
  VICII->SPRITE0_Y = 80;
  VICII->SPRITE0_COLOR = 14;

  VICII->SPRITE1_X = 60;
  VICII->SPRITE1_Y = 120;
  VICII->SPRITE1_COLOR = 5;

  VICII->SPRITE2_X = 60;
  VICII->SPRITE2_Y = 160;
  VICII->SPRITE2_COLOR = 13;
}

void initSprites() {
  // addres where we have first pointer to sprite
  // last 8 bytes in screen memory
  char *spritesPointer = SCREEN_POINTER + $3f8; // 0x07F8

  // 1 block is 64 bytes
  char spriteBlockNumber = (char)((unsigned int)SPRITES / $40); // 0x0080

  // TODO: calculate data position by sprite number and frames count
  spritesPointer[0] = spriteBlockNumber;
  spritesPointer[1] = spriteBlockNumber + 2;
  spritesPointer[2] = spriteBlockNumber + 4;

  // // write block number on each sprite pointer
  // // to show where are the data to the sprites
  // for (int i = 0; i < numOfSprites; i++) {
  //   spritesPointer[i] = spriteBlockNumber++;
  // }
}

char getNextSpriteFrameBySpriteNumber(int spriteNumber) {
    char currentFrame = spritesCurrentFrame[spriteNumber];
    char framesCount = SPRITES_FRAMES_COUNT[spriteNumber];

    if (currentFrame == framesCount -1) {
      return 0;
    }

    return currentFrame + 1;
}

void updateSpriteFrame(int spriteNumber, char spriteFrame, char spriteDataPosition) {
  char *spritesPointer = SCREEN_POINTER + $3f8; // 0x07F8
  char spriteBlockNumber = (char)((unsigned int)SPRITES / $40);

  spritesPointer[spriteNumber] = spriteBlockNumber + spriteDataPosition + spriteFrame;
}

void initScreenByMap(char* map) {
  for (int i = 0; i < 40*25; i++) {
    byte mapValue = map[i];
    
    SCREEN_POINTER[i] = mapValue;
    COLS[i] = CHARS_COLORS[mapValue];
  }
}

void wait_vblank() {
    char oldRaster;
    do {
        oldRaster = VICII->RASTER;
    } while (VICII->RASTER >= oldRaster);  // czekaj na nową ramkę
}

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