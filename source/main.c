#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //最初の読み込み関連
  SDL_Delay(3000);
  Destroy(); //破棄関連
  SDL_Quit();
  return 0;
}

