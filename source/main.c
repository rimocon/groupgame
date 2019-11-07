#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //最初の読み込み関連
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input();
    }
    SDL_Delay(5);
    MoveChara();
    MoveTriangle();
    RenderWindow();
    //Destroy(); //破棄関連
  }
  return 0;
}

