#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //最初の読み込み関連
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input();
    }
    SDL_Delay(10);
    MoveChara();
    RenderWindow();
    //Destroy(); //破棄関連
    //SDL_Quit();
    SDL_RenderPresent(mainrenderer);
  }
  return 0;
}

