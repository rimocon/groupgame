#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //$B:G=i$NFI$_9~$_4XO"(B
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input(); //$BF~NO(B
    }
    SDL_Delay(5);
    MoveChara(); //$B%-%c%i0\F0(B
    MoveTriangle(); //$B;03Q7A0\F0(B
    RenderWindow(); //$BIA2h(B
    Collision(); //$BEv$?$jH=Dj(B
    //Destroy(); //$BGK4~4XO"(B
  }
  return 0;
}

