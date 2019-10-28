#include "common.h"
inputkeys key; //inputkeys構造体をinputという名前で実体化

void Startup() { //初期化関連(1回だけ呼び出す)
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
    SDL_Quit(); //終了
  }
  mainrenderer = SDL_CreateRenderer(mainwindow,-1,0); //メインウィンドウに対するレンダラー生成
  run = true; //動かす
}

void Input() {
  switch (inputevent.type) {
    /*
    // ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
    case SDL_JOYAXISMOTION:
    //printf("The axis ID of the operated key is %d.\n",inputevent.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
    if(inputevent.jaxis.axis==0){
    //printf("--- Analog-Direction Key: 0 Axis\n");
    if(inputevent.jaxis.value > 0) { //右キーが押されたら
    key.right = 1;
    key.left = 0;
    key.up = 0;
    key.down = 0;
    }
    else if(inputevent.jaxis.value < 0) { //左キーが押されたら
    printf("press left\n");
    key.right = 0;
    key.left = 1;
    key.up = 0;
    key.down = 0;
    }
    else if(inputevent.jaxis.value == 0) { //真ん中にスティックが戻ったら
    printf("reverse center\n");
    key.right = 0;
    key.left = 0;
    key.up = 0;
    key.down = 0;
    }
    }
    else if(inputevent.jaxis.axis==1){
    printf("--- Analag-Direction Key: 1 Axis\n");
    if(inputevent.jaxis.value > 0) { //下キーが押されたら
    printf("press down\n"); //確認用
    key.right = 0;
    key.left = 0;
    key.up = 0;
    key.down = 1;
    }
    else if(inputevent.jaxis.value < 0) { //上キーが押されたら
    printf("press up\n"); //確認用
    key.right = 0;
    key.left = 0;
    key.up = 1;
    key.down = 0;
    }
    else if(inputevent.jaxis.value == 0) { //真ん中にスティックが戻ったら
    printf("reverse center\n");
    key.right = 0;
    key.left = 0;
    key.up= 0;
    key.down= 0;
    }
    }
    else if(inputevent.jaxis.axis==2){
    //	printf("--- Four-Direction Key: Horizontal Axis\n");
    }
    else if(inputevent.jaxis.axis==3){
    //	printf("--- Four-Direction Key: Vertical Axis\n");
    }
    break;
    // ジョイスティックのボタンが押された時
    case SDL_JOYBUTTONDOWN:
    //	printf("The ID of the pressed button is %d.\n", inputevent.jbutton.button); // 押されたボタンのIDを表示（0から）
    // ボタンIDに応じた処理
    if(inputevent.jbutton.button==0){
    //		printf("--- You pressed a button on the joystick.\n");
    }
    if(inputevent.jbutton.button== 5){ //発射ボタンが押された
    //	printf("発射!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    if( games == 0){} //クリア画面と初期画面の入力はきかないように
    else if (games == 1) keys.z = 1; //発射キーが押された
    else if (games == 2) {}
    }

    if(inputevent.jbutton.button == 3){ //終了ボタンが押された
      keys.start = 1;
    }
    if(inputevent.jbutton.button== 11){ //終了ボタンが押された
      run = false;
    }
    break;
    // ボタンが離された時
    case SDL_JOYBUTTONUP:
    //	printf("The ID of the released button is %d.\n",inputevent.jbutton.button); // 離されたボタンのIDを表示（0から）
    // ボタンIDに応じた処理
    if(inputevent.jbutton.button==0){
      //		printf("--- You released a button on the joystick.\n");
    }
    break;
    */
    case SDL_KEYDOWN: // キーボードのキーが押された時
      // 押されたキーの名前等を表示
      printf("The pressed key is %s.\n", SDL_GetKeyName(inputevent.key.keysym.sym));
      printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), inputevent.key.keysym.mod);
      // 押されたキーごとに処理
      switch(inputevent.key.keysym.sym){
        case	SDLK_UP:
          printf("press up\n");
          key.right = 0;
          key.left = 0;
          key.up = 1;
          key.down = 0;
          break;
        case SDLK_DOWN:
          printf("press down\n");
          key.right = 0;
          key.left = 0;
          key.up = 0;
          key.down = 1;
          break;
        case SDLK_RIGHT:
          printf("press right\n");
          key.right = 1;
          key.left = 0;
          key.up = 0;
          key.down = 0;
          break;
        case SDLK_LEFT:
          printf("press left\n");
          key.right = 0;
          key.left = 1;
          key.up = 0;
          key.down = 0;
          break;
        case SDLK_1:
          run = false;
          break;
      }
      break;
  }
}


void Destroy() {
  SDL_DestroyWindow(mainwindow);
  SDL_DestroyRenderer(mainrenderer);
}

void Imageload() {
  
}
  
