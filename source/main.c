#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //$B:G=i$NFI$_9~$_4XO"(B
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input(); //$BF~NO(B
    }
    switch(status){
      case MENUMODE:
        DrawMenu();
        break;
      case GAMEMODE:
        MoveChara(); //$B%-%c%i0\F0(B
        MoveTriangle(); //$B;03Q7A0\F0(B
        RenderWindow(); //$BIA2h(B
        Collision(); //$BEv$?$jH=Dj(B
        MoveChara();
        //Destroy(); //$BGK4~4XO"(B
        SDL_Delay(10);
        break;
      case RESULTMODE:
        //DrawResult();
        break;
    }
  }
  return 0;
}

