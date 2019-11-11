#include "common.h"
#include "func.h"

void Startup()
{
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
    SDL_Quit(); //終了
  }
  run = true; //動かす
  for(int i = 0;i<CAMERA_NUM; i++) {
    /*
       camera[i].dst_rects.x = 1200;
       camera[i].dst_rects.y = 900;
       camera[i].dst_rects.w = 80;
       camera[i].dst_rects.h = 60;
     */
    camera[i].clockwise = true;
    camera[i].tri[0][0] = 640.0;
    camera[i].tri[1][0] = 480.0;
    camera[i].theta[0] = 90.0;
    camera[i].theta[1] = 120.0;
  }
  mainrenderer = SDL_CreateRenderer(mainwindow, -1, 0); //メインウィンドウに対するレンダラー生成
  Imageload();
  MakeMap();
}

void Input()
{
  switch (inputevent.type)
  {
    // ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
    case SDL_JOYAXISMOTION:
      printf("The axis ID of the operated key is %d.\n",inputevent.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
      if(inputevent.jaxis.axis==0){
        printf("--- Analog-Direction Key: 0 Axis\n");
        if(inputevent.jaxis.value > 0) { //右キーが押されたら
          key.right = 1;
          key.left = 0;
        }
        else if(inputevent.jaxis.value < 0) { //左キーが押されたら
          printf("press left\n");
          key.right = 0;
          key.left = 1;
        }
        else if(inputevent.jaxis.value == 0) { //真ん中にスティックが戻ったら
          printf("reverse center\n");
          key.right = 0;
          key.left = 0;
        }
      }
      else if(inputevent.jaxis.axis==1){
        printf("--- Analag-Direction Key: 1 Axis\n");
        if(inputevent.jaxis.value > 0) { //下キーが押されたら
          printf("press down\n"); //確認用
          key.up = 0;
          key.down = 1;
        }
        else if(inputevent.jaxis.value < 0) { //上キーが押されたら
          printf("press up\n"); //確認用
          key.up = 1;
          key.down = 0;
        }
        else if(inputevent.jaxis.value == 0) { //真ん中にスティックが戻ったら
          printf("reverse center\n");
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
  }
}

void Destroy()
{
  SDL_DestroyWindow(mainwindow);
  SDL_DestroyRenderer(mainrenderer);
}

void Imageload()
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer); // 設定した描画色(白)でレンダラーをクリア

  SDL_Surface *s; // サーフェイスを一時的に保存する変数

  // 順番的に先に背景を描画する必要あり

  // 「kotei_objects」構造体（金塊、カメラ、棚、出入り口）にグローバル変数で設定した位置、画像をに設定する
  int i= 0;
  for(int j = 0; j<KINKAI_NUM; j++){
    printf("j = %d\n",j);
    kotei_objects[i].type = TYPE_KINKAI;
    s = IMG_Load(imgfiles[TYPE_KINKAI]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    kotei_objects[i].dst_rect =kinkai_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    i++; // 金塊、カメラ、棚、出入り口のデータをkotei_objectsに保存するためにiをインクリメント
  }
  for(int j = 0; j<SHELF_NUM; j++){
    printf("j = %d\n",j);
    kotei_objects[i].type = TYPE_SHELF;
    s = IMG_Load(imgfiles[TYPE_SHELF]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    kotei_objects[i].dst_rect = shelf_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    i++;
  }

  for(int j = 0; j<ENTRANCE_NUM; j++){
    printf("j = %d\n",j);
    kotei_objects[i].type = TYPE_ENTRANCE;
    s = IMG_Load(imgfiles[TYPE_ENTRANCE]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    printf("j = %d\n",j);
    kotei_objects[i].dst_rect = entrance_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    i++;
  }
  for(int j = 0; j<CAMERA_NUM; j++){
    printf("j = %d\n",j);
    kotei_objects[i].type = TYPE_CAMERA;
    s = IMG_Load(imgfiles[TYPE_CAMERA]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    kotei_objects[i].dst_rect =camera_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
  }
  for(i=0; i<PLAYER_NUM; i++){
    player[i].type = TYPE_PLAYER;
    s = IMG_Load(imgfiles[TYPE_PLAYER]);
    player[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
    player[i].src_rect.x = 0;
    player[i].src_rect.y = 0;
    player[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    player[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    player[i].dst_rect = player_dst_rects[i];
    SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    player[i].speed = PLAYER_SPEED;
  }
}

void MoveTriangle()
{ 
  //tri[0][1] = x2
  //tri[1][1] = y2
  //tri[0][2] = x3
  //tri[1][2] = y3
  for (int i = 0; i<CAMERA_NUM;i++) {  //カメラの数だけ繰り返す
    if ( camera[i].theta[0] < 90 || camera[i].theta[1] < 90 ) {
      camera[i].clockwise = false; //反時計回り
    }
    else if(camera[i].theta[0] > 270 || camera[i].theta[1] >270) {
      camera[i].clockwise = true; //時計回り
    }
    if (camera[i].clockwise) {
      camera[i].theta[0]--; 
      camera[i].theta[1]--;
    //構造体playerに、プレイヤーの情報を格納
    for(i=0; i<PLAYER_NUM; i++){
      player[i].type = TYPE_PLAYER;
      s = IMG_Load(imgfiles[TYPE_PLAYER]);
      player[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
      player[i].src_rect.x = 0;
      player[i].src_rect.y = 0;
      player[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
      player[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
      player[i].dst_rect = player_dst_rects[i];
      SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
      player[i].speed = PLAYER_SPEED; // ヘッダで指定した定数をプレイヤーの移動スピードとして設定
    }
    //構造体enemyに、敵の情報を格納
    for(i=0; i<ENEMY_NUM; i++){
      enemy[i].type = TYPE_ENEMY;
      s = IMG_Load(imgfiles[TYPE_ENEMY]);
      enemy[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer,s);
      enemy[i].src_rect.x = 0;
      enemy[i].src_rect.y = 0;
      enemy[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
      enemy[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
      enemy[i].dst_rect = enemy_dst_rects[i];
      SDL_RenderCopy(mainrenderer, enemy[i].image_texture, &enemy[i].src_rect, &enemy[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
      enemy[i].speed = PLAYER_SPEED; // ヘッダで指定した定数をプレイヤーの移動スピードとして設定
      enemy[i].isgodest = false;
      enemy[i].look_angle = enemy_lookangles[i];
    //       if(enemy_dst_rects[i].x > enemy_destination[i][0] && enemy_dst_rects[i].y > enemy_destination[i][1]) enemy[i].movemode = 0;
    // else if(enemy_dst_rects[i].x > enemy_destination[i][0] && enemy_dst_rects[i].y < enemy_destination[i][1]) enemy[i].movemode = 1;
    // else if(enemy_dst_rects[i].x < enemy_destination[i][0] && enemy_dst_rects[i].y > enemy_destination[i][1]) enemy[i].movemode = 2;
    // else if(enemy_dst_rects[i].x < enemy_destination[i][0] && enemy_dst_rects[i].y < enemy_destination[i][1]) enemy[i].movemode = 3;
    }
    else{
      camera[i].theta[0]++;
      camera[i].theta[1]++;
    }
    camera[i].tri[0][1] = camera[i].tri[0][0] + sin(camera[i].theta[0]*M_PI / 180.0)*130; //x2の計算
    camera[i].tri[1][1] = camera[i].tri[1][0] + cos(camera[i].theta[0]*M_PI / 180.0)*130; //y2の計算
    camera[i].tri[0][2] = camera[i].tri[0][0] + sin(camera[i].theta[1]*M_PI / 180.0)*130; //x3の計算
    camera[i].tri[1][2] = camera[i].tri[1][0] + cos(camera[i].theta[1]*M_PI / 180.0)*130; //x3の計算
  }
}
void MoveChara()
{
  int move_distance = PLAYER_SPEED ;
  if(key.left){ //左入力
    player[0].dst_rect.x -= move_distance;
    if(player[0].dst_rect.x <0) player[0].dst_rect.x = 0;
  }
  if(key.right){ //右入力
    player[0].dst_rect.x += move_distance;
    if(player[0].dst_rect.x > WINDOWWIDTH - player[0].dst_rect.w) player[0].dst_rect.x = WINDOWWIDTH - player[0].dst_rect.w; 
  }
  if(key.up){ //上入力
    player[0].dst_rect.y -= move_distance;
    if(player[0].dst_rect.y < 0) player[0].dst_rect.y = 0; 

  }
  if(key.down){ //下入力
    player[0].dst_rect.y += move_distance;
    if(player[0].dst_rect.y > WINDOWHEIGHT - player[0].dst_rect.h) player[0].dst_rect.y = WINDOWHEIGHT - player[0].dst_rect.h; 
  }
}

void RenderWindow(void) //画面の描画(イベントが無い時)
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer);       // 設定した描画色(白)でレンダラーをクリア
  for(int i=0; i<KOTEI_OBJECT_NUM; i++){
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); //固定オブジェクトをレンダーに出力(毎回描画しないといけない？)
  }
  for(int i=0; i<PLAYER_NUM; i++){
    SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); //プレイヤーをレンダーに出力
  }
  for(int i=0; i<ENEMY_NUM; i++){
    SDL_RenderCopy(mainrenderer, enemy[i].image_texture, &enemy[i].src_rect, &enemy[i].dst_rect); //敵をレンダーに出力
  }
  //filledCircleColor(mainrenderer, circle_x, circle_y, 9, 0xff0000ff); //丸の描画
  for(int i = 0;  i<CAMERA_NUM; i++){
    filledTrigonColor(mainrenderer,camera[i].tri[0][0],camera[i].tri[1][0],camera[i].tri[0][1],camera[i].tri[1][1],camera[i].tri[0][2],camera[i].tri[1][2],0xff0000ff);
  }
  // filledTrigonColor(mainrenderer,640,480,700,400,580,400,0xff0000ff);
  SDL_RenderPresent(mainrenderer);              // 描画データを表示
}

void Collision() {
  //00,10,01,11->00,10,02,12->01,11,01,11->01,11,02,12で判定
  //カメラの判定
  for (int i = 0; i < CAMERA_NUM;i++){ //カメラの数だけ
    for(int j = 0; j < 2; j++) { 
      for(int k = 1; k < 3; k++) {
        bool judge = SDL_IntersectRectAndLine(&player[0].dst_rect,
            &camera[i].tri[0][j],
            &camera[i].tri[1][j],
            &camera[i].tri[0][k],
            &camera[i].tri[1][k]);
        if(judge) {
          run = false;
          break;
        }
      }
    }
  }
  //ここまでカメラの判定 
}

void MoveChara()
{
  // プレイヤーの移動
  int move_distance = PLAYER_SPEED * 2; // プレイヤーの移動距離、設定したスピード

  //キー入力状態に応じて移動
  if (key.left)
  {
    player[0].dst_rect.x -= move_distance;
  }
  else if (key.right)
  {
    player[0].dst_rect.x += move_distance;
  }
  else if (key.up)
  {
    player[0].dst_rect.y -= move_distance;
  }
  else if (key.down)
  {
    player[0].dst_rect.y += move_distance;
  }

  //棚との衝突判定
  for (int i = 0; i < KOTEI_OBJECT_NUM; i++)
  {
    if (SDL_HasIntersection(&(kotei_objects[i].dst_rect), &(player[0].dst_rect))) // プレイヤーと固定オブジェクトが重なった時
    {
      if (kotei_objects[i].type != TYPE_SHELF) // 棚以外とぶつかったときは無視
        break;

      // ぶつかったぶんの距離プレイヤーの位置を戻す
      if (key.left)
      {
        player[0].dst_rect.x += move_distance;
      }
      else if (key.right)
      {
        player[0].dst_rect.x -= move_distance;
      }
      else if (key.up)
      {
        player[0].dst_rect.y += move_distance;
      }
      else if (key.down)
      {
        player[0].dst_rect.y -= move_distance;
      }
    }
  }

  //敵キャラの移動
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    switch (enemy[i].look_angle)
    {
    case 0:
      enemy[i].dst_rect.y -= ENEMY_SPEED;
      break;
    case 90:
      enemy[i].dst_rect.x += ENEMY_SPEED;
      break;
    case 180:
      enemy[i].dst_rect.y += ENEMY_SPEED;
      break;
    case 270:
      enemy[i].dst_rect.x -= ENEMY_SPEED;
      break;
    }
  }
}

void MakeMap()
{
  /* マップの読み込みと配置 */
    int i, j, k=0,mt;
    SDL_Surface* s;
    SDL_Rect src = {0, 0, MAP_CHIPSIZE, MAP_CHIPSIZE};
    SDL_Rect dst = {0};
    for (j = 0; j < MAP_HEIGHT; j++, dst.y += MAP_CHIPSIZE)
    {
      dst.x = 0;
      for (i = 0; i < MAP_WIDTH; i++, dst.x += MAP_CHIPSIZE)
      {
        mt = map0[j][i];
        if (mt == MT_SHELF)
        {
          printf("j = %d\n", j);
          kotei_objects[k].type = TYPE_SHELF;
          s = IMG_Load(imgfiles[TYPE_SHELF]);
          kotei_objects[k].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
          kotei_objects[k].src_rect.x = 0;
          kotei_objects[k].src_rect.y = 0;
          kotei_objects[k].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
          kotei_objects[k].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定

          kotei_objects[k].dst_rect.x = dst.x; // マップで指定された場所に出力されるように設定
          kotei_objects[k].dst_rect.y = dst.y;
          kotei_objects[k].dst_rect.w = MAP_CHIPSIZE; // 幅、高さはCHIPSIZEにする
          kotei_objects[k].dst_rect.h = MAP_CHIPSIZE;
          SDL_RenderCopy(mainrenderer, kotei_objects[k].image_texture, &kotei_objects[k].src_rect, &kotei_objects[k].dst_rect); // テクスチャからレンダラーに出力
          k++;
        }
        // else if (mt == MT_ENEMY)
        // {
        //   enemy_count++;
        // }

        // src.x = mt * MAP_ChipSize;
        // src.y = 0;
        // if (0 > SDL_BlitSurface(img, &src, map, &dst))
        // {
        //   ret = PrintError(SDL_GetError());
        // }

        // src.y = MAP_ChipSize;
        // if (0 > SDL_BlitSurface(img, &src, gGame.mapMask, &dst))
        // {
        //   ret = PrintError(SDL_GetError());
        // }
      }
    }
    /* マップはテクスチャに */
    // if (NULL == (gGame.map = SDL_CreateTextureFromSurface(gGame.render, map)))
    // {
    //   ret = PrintError(SDL_GetError());
    // }
  }
}


