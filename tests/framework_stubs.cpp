#include "../framework/Framework.h"

class Sprite {};

Sprite* createSprite(const char*) { return nullptr; }

void drawSprite(Sprite*, int, int) {}

void getSpriteSize(Sprite*, int& w, int& h) {
  w = 0;
  h = 0;
}

void setSpriteSize(Sprite*, int, int) {}

void destroySprite(Sprite*) {}

void drawTestBackground() {}

void getScreenSize(int& w, int& h) {
  w = 390;
  h = 844;
}

unsigned int getTickCount() { return 0U; }

void showCursor(bool) {}

int run(Framework*) { return 0; }