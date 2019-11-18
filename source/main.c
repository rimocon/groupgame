#include "common.h"

int main (int argc, char *argv[]) {
  Startup(); //最初の読み込み関連
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input(); //入力
    }
    switch(status){
      case MENUMODE:
        DrawMenu();
        break;
      case GAMEMODE:
        MoveChara(); //キャラ移動
        MoveTriangle(); //三角形移動
        RenderWindow(); //描画
        Collision(); //当たり判定
        MoveChara();
        //Destroy(); //破棄関連
        SDL_Delay(10);
        break;
      case RESULTMODE:
        //DrawResult();
        break;
    }
  }
  return 0;
}

