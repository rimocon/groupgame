#include "common.h"

void Startup() {
  /*
  // SDL関連すべて初期化,失敗したら終了
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) SDL_Quit(); 

  if(SDL_NumJoysticks() > 0) {
  joystick = SDL_JoystickOpen(0); // ジョイスティックを開く
  printf("%s\n", SDL_JoystickName(joystick)); //開いたジョイスティック名を表示
  }
  // ジョイスティックが開けなかったら
  if(!joystick) {
  printf("failed to open joystick.\n");
  exit(-1);
  }
   */

  if(SDL_Init(SDL_INIT_VIDEO) == -1) SDL_Quit();

  mainwindow = SDL_CreateWindow( //ウィンドウ生成
      "test",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOWWIDTH,
      WINDOWHEIGHT,
      0
      );
  if(mainwindow == NULL){ //ウィンドウを生成できなかったら
    printf("ウィンドウを生成できませんでした\n");
  }
  mainrenderer = SDL_CreateRenderer(mainwindow,-1,0); //メインウィンドウに対するレンダラー生成
}

void Destroy() {
  SDL_DestroyWindow(mainwindow);
  SDL_DestroyRenderer(mainrenderer);
}
