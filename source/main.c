#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //$B:G=i$NFI$_9~$_4XO"(B
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input();
    }
    SDL_Delay(10);
    MoveChara();
    RenderWindow();
    //Destroy(); //$BGK4~4XO"(B
    //SDL_Quit();
    SDL_RenderPresent(mainrenderer);
  }
  return 0;
}

