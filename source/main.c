#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //$B:G=i$NFI$_9~$_4XO"(B
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input();
    }
    SDL_Delay(5);
    MoveChara();
    MoveTriangle();
    RenderWindow();
    //Destroy(); //$BGK4~4XO"(B
  }
  return 0;
}

