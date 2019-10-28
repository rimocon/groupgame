#include "common.h"

void Startup()
{
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
  int w_kinkai = 400; //金塊の幅
  int h_kinkai = 384; //金塊の高さ
  int w_shelf = 239; //棚の幅
  int h_shelf = 544; //棚の高さ

  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    SDL_Quit();

  mainwindow = SDL_CreateWindow( //ウィンドウ生成
      "test",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOWWIDTH,
      WINDOWHEIGHT,
      0);
  if (mainwindow == NULL)
  { //ウィンドウを生成できなかったら
    printf("ウィンドウを生成できませんでした\n");
  }

  mainrenderer = SDL_CreateRenderer(mainwindow, -1, 0); //メインウィンドウに対するレンダラー生成

  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer); // 設定した描画色(白)でレンダラーをクリア

  SDL_Surface *image_kinkai = IMG_Load("kinkai.png");
  SDL_Surface *image_shelf = IMG_Load("shelf.png");
  SDL_Texture *texture_kinkai = SDL_CreateTextureFromSurface(mainrenderer, image_kinkai);
  SDL_Texture *texture_shelf = SDL_CreateTextureFromSurface(mainrenderer, image_shelf);
  SDL_Rect src_rect_kinkai = {0, 0, w_kinkai, h_kinkai}; // 転送元画像の領域
  /*画像の転送先の座標と領域転送先レンダラー 
  　転送元テクスチャ 転送元領域転送先領域*/
  SDL_Rect dst_rect_kinkai = {1000, 100, 100, 100};
  SDL_RenderCopy(mainrenderer, texture_kinkai, &src_rect_kinkai, &dst_rect_kinkai);
  SDL_Rect src_rect_shelf = {0, 0, w_shelf, h_shelf}; // 転送元画像の領域
  SDL_Rect dst_rect_shelf = {400, 100, 46, 108};
  SDL_RenderCopy(mainrenderer, texture_shelf, &src_rect_shelf, &dst_rect_shelf);
  SDL_RenderPresent(mainrenderer); // 描画データを表示
}

void Destroy()
{
  SDL_DestroyWindow(mainwindow);
  SDL_DestroyRenderer(mainrenderer);
}
