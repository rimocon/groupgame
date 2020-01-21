#include "../common.h"
#include "../constants.h"
#include "func.h"

/*
   static int num_clients;
   static int myid;
   static int sock;
   static int num_sock;
   static fd_set mask; //FD集合を表す構造体
   static CLIENT clients[MAX_NUM_CLIENTS];
 */
void Startup()
{
  // SDL関連すべて初期化,失敗したら終了
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    SDL_Quit();

  if (SDL_NumJoysticks() > 0)
  {
    joystick = SDL_JoystickOpen(0);             // ジョイスティックを開く
    printf("%s\n", SDL_JoystickName(joystick)); //開いたジョイスティック名を表示
  }
  // ジョイスティックが開けなかったら
  if (!joystick)
  {
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
  kinkai_flag = true;                                           //金塊は最初は、配置されている
  kinkai_keep_flag = false;                                     //最初は、プレイヤーは金塊を保持していない
  hacking_flag = false; //最初ハッキングはされていない
  player_flag[0] = true;                                        //プレイヤー1 は最初は、生存
  player_flag[1] = true;                                        //プレイヤー2 は最初は、生存
  player_flag[2] = true;                                        //プレイヤー3 は最初は、生存
  mainrenderer = SDL_CreateRenderer(mainwindow, -1, 0);         //メインウィンドウに対するレンダラー生成
  TTF_Init();                                                   //ttfを初期化
  japanesefont = TTF_OpenFont("fonts-japanese-gothic.ttf", 80); //フォント読み込み
  SetCamera();                                                  //カメラの値設定
  Fontload();                                                   //フォント読み込み
  Imageload();                                                  //画像読み込み
  for (int i = 0; i < CAMERA_NUM; i++)
  {
    printf("camera %d.x = %d\n", i, camera[i].dst_rect.x);
    printf("camera %d.y = %d\n", i, camera[i].dst_rect.y);
    printf("camera %d.w = %d\n", i, camera[i].dst_rect.w);
    printf("camera %d.h = %d\n", i, camera[i].dst_rect.h);
  }
  for (int i = 0; i<PLAYER_NUM;i++){
    player[i].flag_hack_start = false;
  }
  MakeMap();
  status = MENUMODE; //メニューモードに状態を設定
  run = true; //動かす
  up = false;
  down = false;
  same_place_flag = 0;
  random_start_flag = 0;
  stay_time = 0;
  random_start = 0;
  random_time = 0;

  /*
  player[0].key.left = player[0].key.right = player[0].key.up = player[0].key.down  = 0;
  player[1].key.left = player[1].key.right = player[1].key.up = player[1].key.down  = 0;
  player[2].key.left = player[2].key.right = player[2].key.up = player[2].key.down  = 0;
  */
}

void Input()
{
  switch (inputevent.type)
  {
    // ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
    case SDL_JOYAXISMOTION:
      printf("kinkai_keep_flag = %d\n", kinkai_keep_flag);
      printf("The axis ID of the operated key is %d.\n", inputevent.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
      if (inputevent.jaxis.axis == 0)
      {
        printf("--- Analog-Direction Key: 0 Axis\n");
        if (inputevent.jaxis.value > 0)
        { //右キーが押されたら
          //key.right = 1;
          //key.left = 0;
          //スティック操作(右),コマンド送信される
          joystick_send(3);
        }
        else if (inputevent.jaxis.value < 0)
        { //左キーが押されたら
          //key.right = 0;
          //key.left = 1;
          //スティック操作(左),コマンド送信される
          joystick_send(4);
        }
        else if (inputevent.jaxis.value == 0)
        { //真ん中にスティックが戻ったら
          //key.right = 0;
          //key.left = 0;
          //スティック操作(真ん中),コマンド送信される
          joystick_send(7);
        }
      }
      else if (inputevent.jaxis.axis == 1)
      {
        printf("--- Analag-Direction Key: 1 Axis\n");
        if (inputevent.jaxis.value > 0)
        { //下キーが押されたら
          //key.up = 0;
          //key.down = 1;
          //スティック操作(下),コマンド送信される
          joystick_send(6);
          printf("下\n");
        }
        else if (inputevent.jaxis.value < 0)
        { //上キーが押されたら
          //key.up = 1;
          //key.down = 0;
          //スティック操作(上),コマンド送信される
          joystick_send(5);
          printf("上\n");
        }
        else if (inputevent.jaxis.value == 0)
        { //真ん中にスティックが戻ったら
          //key.up = 0;
          //key.down = 0;
          //スティック操作(真ん中),コマンド送信される
          joystick_send(8);
        }
      }
      else if (inputevent.jaxis.axis == 2)
      {
        //	printf("--- Four-Direction Key: Horizontal Axis\n");
      }
      else if (inputevent.jaxis.value < 0)
      { //左キーが押されたら
        //key.right = 0;
        //key.left = 1;
        //スティック操作(左),コマンド送信される
        joystick_send(4);
      }
      else if (inputevent.jaxis.value == 0)
      { //真ん中にスティックが戻ったら
        //key.right = 0;
        //key.left = 0;
        //スティック操作(真ん中),コマンド送信される
        joystick_send(7);
      }
      else if (inputevent.jaxis.axis == 1)
      {
        printf("--- Analag-Direction Key: 1 Axis\n");
        if (inputevent.jaxis.value > 0)
        { //下キーが押されたら
          //key.up = 0;
          //key.down = 1;
          //スティック操作(下),コマンド送信される
          joystick_send(6);
          printf("下\n");
        }
        else if (inputevent.jaxis.value < 0)
        { //上キーが押されたら
          //key.up = 1;
          //key.down = 0;
          //スティック操作(上),コマンド送信される
          joystick_send(5);
          printf("上\n");
        }
        else if (inputevent.jaxis.value == 0)
        { //真ん中にスティックが戻ったら
          //key.up = 0;
          //key.down = 0;
          //スティック操作(真ん中),コマンド送信される
          joystick_send(8);
        }
      }
      break;
      // ジョイスティックのボタンが押された時
    case SDL_JOYBUTTONDOWN:
      //	printf("The ID of the pressed button is %d.\n", inputevent.jbutton.button); // 押されたボタンのIDを表示（0から）
      // ボタンIDに応じた処理
      if (inputevent.jbutton.button == 11)
      {
        run = false;
      }
      if (inputevent.jbutton.button == 0) //ハッキングボタン
      {
        if(player[myid].hack>0){
          joystick_send(10); // ハッキングゲージスタート
        }
      }
      //金塊を取る
      if (inputevent.jbutton.button == 3)
      {
        for (int i = 0; i < kotei_object_num; i++)
        {
          if (kotei_objects[i].type == TYPE_KINKAI)
          {
            if (player[myid].dst_rect.x >= kotei_objects[i].dst_rect.x && player[myid].dst_rect.x <= kotei_objects[i].dst_rect.x + MAP_CHIPSIZE)
            {
              if (player[myid].dst_rect.y >= kotei_objects[i].dst_rect.y && player[myid].dst_rect.y <= kotei_objects[i].dst_rect.y + MAP_CHIPSIZE)
              {
                //kinkai_flag = false;
                //スティック操作がされた時、金塊情報などのデータ送信される
                joystick_send(1);
              }
            }
          }
          player[myid].key.a = 1;
        }
      }

      //2ボタンが押された時(jbuttonだと1)
      if (inputevent.jbutton.button == 1)
      {
        joystick_send(12);
      }
      break;
      // ボタンが離された時
    case SDL_JOYBUTTONUP:
      //	printf("The ID of the released button is %d.\n",inputevent.jbutton.button); // 離されたボタンのIDを表示（0から）
      // ボタンIDに応じた処理
      // 2ボタンが離された
      if (inputevent.jbutton.button == 1)
      {
        joystick_send(14);
      }
      if (inputevent.jbutton.button == 0) //ハッキングボタン
      {
        if(player[myid].hack > 0){
          if(SDL_GetTicks() - player[myid].inputtime > HACKTIME){ //2秒以上経過した場合
            joystick_send(9); //ハッキングの処理
          }
          else{ //途中でキャンセルされた場合
          gauge = 0;
          joystick_send(11); //ハッキングキャンセルの処理
          }
        }
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
  SDL_Surface *s;
  for (int i = 0; i < BACKGROUND_NUM; i++)
  { //背景画像読み込み
    printf("backgroundlord \n");
    s = IMG_Load(imgfiles[TYPE_BACKGROUND]);
    background[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    background[i].src_rect.x = 0;
    background[i].src_rect.y = 0;
    background[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    background[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    background[i].dst_rect.x = 0;
    background[i].dst_rect.y = 0;
    background[i].dst_rect.w = WINDOWWIDTH;
    background[i].dst_rect.h = WINDOWHEIGHT;
  }
  for (int i = 0; i < CAMERA_NUM; i++)
  {
    s = IMG_Load(imgfiles[TYPE_CAMERA]);
    camera[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    camera[i].src_rect.x = 0;
    camera[i].src_rect.y = 0;
    camera[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    camera[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    camera[i].dst_rect = camera_dst_rects[i];
    printf("camera %d.x = %d\n", i, camera[i].dst_rect.x);
  }
  printf("imageload\n");
}

void MoveTriangle()
{

  for (int i = 0; i < CAMERA_NUM; i++)
  { //カメラの数だけ繰り返す
    if (camera[i].theta[2] < 120 - camera[i].angle - 90)
    {
      camera[i].clockwise = false; //反時計回り
    }
    else if (camera[i].theta[2] > 240 - camera[i].angle - 90)
    {
      camera[i].clockwise = true; //時計回り
    }
    //ハッキング処理があった場合ここで処理を止める.
    if (!hacking_flag){
      if (camera[i].clockwise) {
        camera[i].theta[2]--; //三角形の頂点の座標の角度を変える
      }
      else
      {
        camera[i].theta[2]++;
      }
    }
    camera[i].theta[0] = camera[i].theta[2] + 15; //三角形の残り2点の角度を変える
    camera[i].theta[1] = camera[i].theta[2] - 15;
    Rotation(camera_dst_rects[i].x + camera_dst_rects[i].w - camera_dst_rects[i].w / 4,
        camera_dst_rects[i].y + camera_dst_rects[i].h / 2,
        camera_dst_rects[i].x + camera_dst_rects[i].w / 2,
        camera_dst_rects[i].y + camera_dst_rects[i].h / 2,
        90 - camera[i].theta[2],
        &camera[i].tri[0][0],
        &camera[i].tri[1][0]); //右端縦真ん中の座標を中心座標から三角形の頂点の座標角度分回転(回転座標系のとり方が違うので90から引いて正規化)
    //三角形の残り2点の位置計算
    camera[i].tri[0][1] = camera[i].tri[0][0] + sin(camera[i].theta[0] * M_PI / 180.0) * 250; //x2の計算
    camera[i].tri[1][1] = camera[i].tri[1][0] + cos(camera[i].theta[0] * M_PI / 180.0) * 250; //y2の計算
    camera[i].tri[0][2] = camera[i].tri[0][0] + sin(camera[i].theta[1] * M_PI / 180.0) * 250; //x3の計算
    camera[i].tri[1][2] = camera[i].tri[1][0] + cos(camera[i].theta[1] * M_PI / 180.0) * 250; //x3の計算
  }

  //敵の視界を動かす処理
  int origin_x, origin_y;
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    origin_x = enemy[i].dst_rect.x + enemy[i].dst_rect.w / 2;
    origin_y = enemy[i].dst_rect.y + enemy[i].dst_rect.h / 2;
    origin_x += 13 * sin(enemy[i].prev_angle * M_PI / 180);
    origin_y += 13 * -cos(enemy[i].prev_angle * M_PI / 180);
    enemy[i].tri[0][0] = origin_x;
    enemy[i].tri[1][0] = origin_y;
    // 20度回転させた敵の視界の当たり判定を作る
    int vision[2] = {enemy[i].prev_angle - 20, enemy[i].prev_angle + 20};
    for (int j = 0; j < 2; j++)
    {
      if (vision[j] < 0)
        vision[j] += 360;
      else if (vision[j] >= 360)
        vision[j] -= 360;
      // 200が視界の範囲
      enemy[i].tri[0][j + 1] = origin_x + 200 * sin(vision[j] * M_PI / 180);
      enemy[i].tri[1][j + 1] = origin_y + 200 * -cos((vision[j]) * M_PI / 180);
    }
  }
}

void RenderWindow(void) //画面の描画(イベントが無い時)
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer);                            // 設定した描画色(白)でレンダラーをクリア
  for (int i = 0; i < kotei_object_num; i++)
  {
    //描画対象が金塊で、金塊が地面に設置されていなければ、描画しない
    if (!(kotei_objects[i].type == TYPE_KINKAI && kinkai_flag == false))
    {
      SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); //固定オブジェクトをレンダーに出力(毎回描画しないといけない？)
    }
  }
  // 催涙スプレーの描画
  for (int i = 0; i < PLAYER_NUM; i++)
  {
    if (player_flag[i] == true)
    {
      SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); //プレイヤーをレンダーに出力
      if (player[i].spray_flag == 1)
      {
        SDL_RenderCopyEx(mainrenderer, player[i].spray_texture, &player[i].spray_src_rect, &player[i].spray_dst_rect, player[i].look_angle - 90, &player[i].spray_origin, SDL_FLIP_NONE);
        // lineColor(mainrenderer,player[i].spray_hitlines[0][0],player[i].spray_hitlines[1][0],player[i].spray_hitlines[0][1],player[i].spray_hitlines[1][1], 0xff00ff00); // 催涙スプレーの当たり判定、デバッグ用
        // lineColor(mainrenderer,player[i].spray_hitlines[0][0],player[i].spray_hitlines[1][0],player[i].spray_hitlines[0][2],player[i].spray_hitlines[1][2], 0xff00ff00); // 催涙スプレーの当たり判定、デバッグ用
        // lineColor(mainrenderer,player[i].spray_hitlines[0][0],player[i].spray_hitlines[1][0],player[i].spray_hitlines[0][3],player[i].spray_hitlines[1][3], 0xff00ff00); // 催涙スプレーの当たり判定、デバッグ用
      }
      //催涙スプレーのゲージ描画
      double unit = 500 / (double)SPRAY_TIME;                                           // 500pxがスプレーのMAX
      int line_h = 20;                                                                  // スプレーの残りを表すゲージの縦幅
      int spraygauge = WINDOWWIDTH - abs(player[i].spraytime - SPRAY_TIME) * unit;      // スプレーの残りを描画するために値を変換
      SDL_Rect dst = {WINDOWWIDTH - 534, (WINDOWHEIGHT - 65) + i * line_h + 5, 20, 20}; // ゲージ横のプレイヤー画像
      SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &dst); // プレイヤー画像をレンダーに出力
      if (player[i].spraytime > 0)
      {
        boxColor(mainrenderer, WINDOWWIDTH - 500, (WINDOWHEIGHT - 65) + i * line_h + 5, spraygauge, (WINDOWHEIGHT - 65) + (i + 1) * line_h, 0xff00ffff); // スプレーの残りをゲージで描画
      }
    }
  }
  // 敵の描画
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    SDL_RenderCopy(mainrenderer, enemy[i].image_texture, &enemy[i].src_rect, &enemy[i].dst_rect); //敵をレンダーに出力
    // lineColor(mainrenderer,enemy[i].tri[0][0],enemy[i].tri[1][0],enemy[i].tri[0][1],enemy[i].tri[1][1], 0xff00ff00); // 当たり判定、デバッグ用
    // lineColor(mainrenderer,enemy[i].tri[0][0],enemy[i].tri[1][0],enemy[i].tri[0][2],enemy[i].tri[1][2], 0xff00ff00); // 当たり判定、デバッグ用
    // lineColor(mainrenderer,enemy[i].tri[0][1],enemy[i].tri[1][1],enemy[i].tri[0][2],enemy[i].tri[1][2], 0xff00ff00); // 当たり判定、デバッグ用
    if (enemy[i].flag_sairui == false)
      filledTrigonColor(mainrenderer, enemy[i].tri[0][0], enemy[i].tri[1][0], enemy[i].tri[0][1], enemy[i].tri[1][1], enemy[i].tri[0][2], enemy[i].tri[1][2], 0xff0000ff);
  }

  for(int i = 0;  i<CAMERA_NUM; i++){
    filledTrigonColor(mainrenderer,camera[i].tri[0][0],camera[i].tri[1][0],camera[i].tri[0][1],camera[i].tri[1][1],camera[i].tri[0][2],camera[i].tri[1][2],0xff0000ff);
    SDL_RenderCopyEx(mainrenderer, camera[i].image_texture, &camera[i].src_rect, &camera[i].dst_rect,90 - camera[i].theta[2],NULL,SDL_FLIP_VERTICAL); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
  }
  for(int i=0; i< PLAYER_NUM; i++){
    if(player[i].flag_hack_start){ //ゲージ描画
      gauge += 0.53;
      if(gauge > 64) {
        gauge = 64;
        boxColor(mainrenderer,player[i].dst_rect.x-20,player[i].dst_rect.y -10,player[i].dst_rect.x -20 + gauge,player[i].dst_rect.y,0xffff0000); //ゲージの枠表示
      }
      else{
        rectangleColor(mainrenderer,player[i].dst_rect.x-20,player[i].dst_rect.y -10,player[i].dst_rect.x+player[i].dst_rect.w + 22,player[i].dst_rect.y,0xff0000ff); //ゲージ表示
        boxColor(mainrenderer,player[i].dst_rect.x-20,player[i].dst_rect.y -10,player[i].dst_rect.x -20 + gauge,player[i].dst_rect.y,0xff0000ff);  //ゲージの枠表示
      }
    }
  }
  SDL_RenderPresent(mainrenderer); // 描画データを表示
}

void Collision()
{
  //00,10,01,11->00,10,02,12->01,11,01,11->01,11,02,12で判定
  //カメラの判定
  for (int i = 0; i < CAMERA_NUM; i++)
  { //カメラの数だけ
    for (int j = 0; j < 2; j++)
    {
      for (int k = 1; k < 3; k++)
      {
        /*カメラの当たり判定に使っているライブラリが、何かに接触すると、その物体に張り付く動作をするライブラリであったので、
          カメラの張り付く前の座標を保持しておき、張り付いた後に、カメラの座標に代入することにより、カメラを固定する。*/
        camera_before[i].tri[0][j] = camera[i].tri[0][j];
        camera_before[i].tri[1][j] = camera[i].tri[1][j];
        camera_before[i].tri[0][k] = camera[i].tri[0][k];
        camera_before[i].tri[1][k] = camera[i].tri[1][k];
        bool judge = SDL_IntersectRectAndLine(&player[myid].dst_rect,
            &camera[i].tri[0][j],
            &camera[i].tri[1][j],
            &camera[i].tri[0][k],
            &camera[i].tri[1][k]);
        //カメラとプレイヤーがぶつかった時
        if (judge && player[myid].flag_kinkai == true)
        {
          player_flag[myid] = false;
          joystick_send(2); //プレイヤーが消えたことが他のクライアントに通知される。
          camera[i].tri[0][j] = camera_before[i].tri[0][j];
          camera[i].tri[1][j] = camera_before[i].tri[1][j];
          camera[i].tri[0][k] = camera_before[i].tri[0][k];
          camera[i].tri[1][k] = camera_before[i].tri[1][k];
          break;
        }
        else{
          camera[i].tri[0][j] = camera_before[i].tri[0][j];
          camera[i].tri[1][j] = camera_before[i].tri[1][j];
          camera[i].tri[0][k] = camera_before[i].tri[0][k];
          camera[i].tri[1][k] = camera_before[i].tri[1][k];
        }
      }
    }
  }
  //ここまでカメラの判定

  //敵の判定
  int tri_before[2][3];
    for (int j = 0; j < ENEMY_NUM; j++)
    {
      tri_before[0][0] = enemy[j].tri[0][0];
      tri_before[1][0] = enemy[j].tri[1][0];
      tri_before[0][1] = enemy[j].tri[0][1];
      tri_before[1][1] = enemy[j].tri[1][1];
      tri_before[0][2] = enemy[j].tri[0][2];
      tri_before[1][2] = enemy[j].tri[1][2];

      if ((SDL_IntersectRectAndLine(&player[myid].dst_rect, &enemy[j].tri[0][0], &enemy[j].tri[1][0], &enemy[j].tri[0][1], &enemy[j].tri[1][1]) ||
           SDL_IntersectRectAndLine(&player[myid].dst_rect, &enemy[j].tri[0][0], &enemy[j].tri[1][0], &enemy[j].tri[0][2], &enemy[j].tri[1][2]) ||
           SDL_IntersectRectAndLine(&player[myid].dst_rect, &enemy[j].tri[0][1], &enemy[j].tri[1][1], &enemy[j].tri[0][2], &enemy[j].tri[1][2])) &&
          enemy[j].flag_sairui == false &&
          player[myid].flag_kinkai == true)
      {
        player_flag[myid] = false; // 他のプレイヤーも消える？
        joystick_send(2);          //プレイヤーが消えたことが他のクライアントに通知される。
        enemy[j].tri[0][0] = tri_before[0][0];
        enemy[j].tri[1][0] = tri_before[1][0];
        enemy[j].tri[0][1] = tri_before[0][1];
        enemy[j].tri[1][1] = tri_before[1][1];
        enemy[j].tri[0][2] = tri_before[0][2];
        enemy[j].tri[1][2] = tri_before[1][2];
      }
      else {
        enemy[j].tri[0][0] = tri_before[0][0];
        enemy[j].tri[1][0] = tri_before[1][0];
        enemy[j].tri[0][1] = tri_before[0][1];
        enemy[j].tri[1][1] = tri_before[1][1];
        enemy[j].tri[0][2] = tri_before[0][2];
        enemy[j].tri[1][2] = tri_before[1][2];
      }
    }
  //ここまで敵の判定
}
void MoveChara()
{
  int move_distance = PLAYER_SPEED * 1;
  //static back_diff_x = 0;
  //static back_diff_y = 0;
  float move; //移動係数
  double distance;
  int min_distance;
  int min_x;
  int min_y;
  static int min_k = 0; //距離が最小のi
  int k;

  static int count = 0;

  /*
     if (same_place_flag == 1)
     {
     random_start = SDL_GetTicks();
     same_place_flag = 2;
     }
     if (SDL_GetTicks() - random_start > 5000)
     {
     same_place_flag = 0;
     random_start = 0;
     }
   */

  for (int i = 0; i < PLAYER_NUM; i++)
  {
    if (player_flag[i] == true)
    {
    if (player[i].key.left == 1 || player[i].key.right == 1)
    {
      if (player[i].key.up == 1 || player[i].key.down == 1)
      {
        move = 0.71f; //移動係数を0.71に設定
        if (player[i].key.right && player[i].key.up)
        {
          player[i].look_angle = 45;
          player[i].src_rect.y = 120;
        }
        else if (player[i].key.right && player[i].key.down)
        {
          player[i].look_angle = 135;
          player[i].src_rect.y = 168;
        }
        else if (player[i].key.left && player[i].key.down)
        {
          player[i].look_angle = 225;
          player[i].src_rect.y = 144;
        }
        else if (player[i].key.left && player[i].key.up)
        {
          player[i].look_angle = 315;
          player[i].src_rect.y = 96;
        }
      }
      else
      {
        move = 1.0f; //斜めじゃなければ1.0に設定
        if (player[i].key.left)
        {
          player[i].look_angle = 270;
          player[i].src_rect.y = 48;
        }
        else if (player[i].key.right)
        {
          player[i].look_angle = 90;
          player[i].src_rect.y = 72;
        }
      }
    }
    else if (player[i].key.up == 1 || player[i].key.down == 1)
    {
      move = 1.0f;

      if (player[i].key.right == 1 || player[i].key.left == 1)
      {
        move = 0.71f; //移動係数を0.71に設定
        if (player[i].key.right && player[i].key.up)
        {
          player[i].look_angle = 45;
          player[i].src_rect.y = 120;
        }
        else if (player[i].key.right && player[i].key.down)
        {
          player[i].look_angle = 135;
          player[i].src_rect.y = 168;
        }
        else if (player[i].key.left && player[i].key.down)
        {
          player[i].look_angle = 225;
          player[i].src_rect.y = 144;
        }
        else if (player[i].key.left && player[i].key.up)
        {
          player[i].look_angle = 315;
          player[i].src_rect.y = 96;
        }
      }
      else
      {
        move = 1.0f; ////斜めじゃなければ1.0に設定
        if (player[i].key.up)
        {
          player[i].look_angle = 0;
          player[i].src_rect.y = 0;
        }
        else if (player[i].key.down)
        {
          player[i].look_angle = 180;
          player[i].src_rect.y = 24;
        }
      }
    }


    if (player[i].key.left == 1 )
    {
      player[i].back_zahyo_x -= player[i].speed * move; //プレイヤーの座標をfloat型で保持
      if (player[i].back_zahyo_x < 0)
      {
        player[i].back_zahyo_x = 0;
      }
      player[i].dst_rect.x = player[i].back_zahyo_x;
    }
    if (player[i].key.right == 1)
    {
      player[i].back_zahyo_x += player[i].speed * move; //プレイヤーの座標をfloat型で保持
      if (player[i].back_zahyo_x > WINDOWWIDTH - player[0].dst_rect.w)
      {
        player[i].back_zahyo_x = WINDOWWIDTH - player[i].dst_rect.w;
      }
      player[i].dst_rect.x = player[i].back_zahyo_x;
    }
    if (player[i].key.up == 1)
    {
      player[i].back_zahyo_y -= player[i].speed * move; //プレイヤーの座標をfloat型で保持
      if (player[i].back_zahyo_y < 0)
      {
        player[i].back_zahyo_y = 0;
      }
      player[i].dst_rect.y = player[i].back_zahyo_y;
    }
    if (player[i].key.down == 1)
    {
      player[i].back_zahyo_y += player[i].speed * move; //プレイヤーの座標をfloat型で保持
      if (player[i].back_zahyo_y > WINDOWHEIGHT - player[0].dst_rect.h)
      {
        player[i].back_zahyo_y = WINDOWHEIGHT - player[i].dst_rect.h;
      }
      player[i].dst_rect.y = player[i].back_zahyo_y;
    }
    /*
       if(player[i].flag_hack_start)
       {
    // ぶつかったぶんの距離プレイヤーの位置を戻す
    if (player[i].key.left)
    {
    player[i].back_zahyo_x += move;
    player[i].dst_rect.x = player[i].back_zahyo_x;
    }
    if (player[i].key.right)
    {
    player[i].back_zahyo_x -= move;
    player[i].dst_rect.x = player[i].back_zahyo_x;
    }
    if (player[i].key.up)
    {
    player[i].back_zahyo_y += move;
    player[i].dst_rect.y = player[i].back_zahyo_y;
    }
    if (player[i].key.down)
    {
    player[i].back_zahyo_y -= move;
    player[i].dst_rect.y = player[i].back_zahyo_y;
    }
    }
     */
    //棚との衝突判定
    for (int j = 0; j < kotei_object_num; j++)
    {
      if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &player[i].dst_rect)) // プレイヤーと固定オブジェクトが重なった時
      {
        if (kotei_objects[j].type == TYPE_SHELF){ // 棚以外とぶつかったときは無視
          // ぶつかったぶんの距離プレイヤーの位置を戻す
          if (player[i].key.left)
          {
            player[i].back_zahyo_x += player[i].speed * move;
            player[i].dst_rect.x = player[i].back_zahyo_x;
          }
          if (player[i].key.right)
          {
            player[i].back_zahyo_x -= player[i].speed * move;
            player[i].dst_rect.x = player[i].back_zahyo_x;
          }
          if (player[i].key.up)
          {
            player[i].back_zahyo_y += player[i].speed * move;
            player[i].dst_rect.y = player[i].back_zahyo_y;
          }
          if (player[i].key.down)
          {
            player[i].back_zahyo_y -= player[i].speed * move;
            player[i].dst_rect.y = player[i].back_zahyo_y;
          }
        }
        else if(kotei_objects[j].type == TYPE_ENTRANCE){
          if(player[i].flag_kinkai == true){
            //gameclear処理
            printf("GAME CLEAR!!\n");
          }
        }
      }
    }
    /*
    //金塊を持っているときに、出口から出た時の処理
    if (player[i].key.down)
    {
      if(player[i].dst_rect.y)
      player[i].back_zahyo_y -= player[i].speed * move;
      player[i].dst_rect.y = player[i].back_zahyo_y;
    }
    */
    if (i == 1)
    {
      printf("通し番号 = %d, x = %d, y = %d\n", count, player[0].dst_rect.x, player[0].dst_rect.y);
      count++;
    }
  }
    else{
      player[i].dst_rect.x = -10000;
      player[i].dst_rect.y = -10000;
    }
  }

  //敵キャラの移動
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    for (int j = 0; j < kotei_object_num; j++)
    {
      SDL_Rect overrap_rect;

      srand((unsigned int)time(NULL)); // MT_RANDOM用に現在時刻の情報で初期化
      int random = rand() % 100;
      //敵の動くタイプによって処理変える
      switch (enemy[i].movetype)
      {
        // 敵が移動床に乗った時の処理
        case MT_MOVING_FLOOR:
          if (SDL_IntersectRect(&kotei_objects[j].dst_rect, &enemy[i].dst_rect, &overrap_rect) &&                                            // 敵が固定オブジェクトに重なる、かつ
              kotei_objects[j].type >= TYPE_ENEMY_MOVING_FLOOR_UL &&                                                                         // 固定オブジェクトが移動床だったとき、かつ
              overrap_rect.w >= enemy[i].dst_rect.w && overrap_rect.h >= enemy[i].dst_rect.h &&                                              // 敵と、移動床が完全に重なって、かつ
              abs((enemy[i].dst_rect.x + enemy[i].dst_rect.w / 2) - (kotei_objects[j].dst_rect.x + kotei_objects[j].dst_rect.w / 2)) <= 2 && // 敵のx座標が移動床の真ん中に近くなって、かつ
              abs((enemy[i].dst_rect.y + enemy[i].dst_rect.h / 2) - (kotei_objects[j].dst_rect.y + kotei_objects[j].dst_rect.h / 2)) <= 2)
          { // 敵のy座標が移動床の真ん中に近くなったとき
            if (enemy[i].prev_overlap_rect.w == 0 && enemy[i].prev_overlap_rect.h == 0)
            {                                                                                    // 前回移動床に乗った時の座標から、MAP_CHIPSIZE分離れているか、または移動床に乗ったのが最初のとき
              ChangeEnemyMoveAngle(&enemy[i], kotei_objects[j].dst_rect, kotei_objects[j].type); // 床のタイプによって、敵の動く方向をかえる
              enemy[i].prev_overlap_rect = overrap_rect;                                         // 前回移動床に乗った時の座標を保存しておく（同じ床で判定して無限ループにならないように）
            }
          }
          // 敵が移動床に乗った時の処理ここまで
          break;

          // 敵がランダムに動く処理
       case MT_RANDOM:
        if (((enemy[i].dst_rect.x + enemy[i].dst_rect.w / 2 + MAP_CHIPSIZE / 2) % MAP_CHIPSIZE <= 2) && ((enemy[i].dst_rect.y + enemy[i].dst_rect.h / 2 + MAP_CHIPSIZE / 2) % MAP_CHIPSIZE <= 2))
        { // 敵がMAP_CHIPSIZE*MAP_CHIPSIZEマスの真ん中に来た時
          if (enemy[i].prev_overlap_rect.w == 0 && enemy[i].prev_overlap_rect.h == 0)
          { // 前回真ん中に来た時の座標から、MAP_CHIPSIZE分離れているか、または移動床に乗ったのが最初のとき
            //ランダムにする処理
            if (random >= 0 && random <= 40)
            {
              enemy[i].move_angle = 0;
            }
            else if (random > 40 && random <= 50)
            {
              enemy[i].move_angle = 90;
            }
            else if (random > 50 && random <= 70)
            {
              enemy[i].move_angle = 180;
            }
            else if (random > 70 && random <= 100)
            {
              enemy[i].move_angle = 270;
            }
            else
            {
              printf("random 値エラー\n");
            }
            enemy[i].prev_overlap_rect = enemy[i].dst_rect; // 前回移動床に乗った時の座標を保存しておく（同じ床で判定して無限ループにならないように）
          }
        }
        break;
      /*
      case MT_TRACKING:
            //追跡してくるNPC
            //現在は、x,y座標に対して、プレイヤー側に寄ってくるようにしているが、
            //x座標または、y座標のどちらか片方のみに設定すると、道の追跡ではなく、
            //道の”とうせんぼ”ができる！
        if (enemy[i].dst_rect.x > player[i].dst_rect.x)
        {
          enemy[i].move_angle = 270;
        }
        if (enemy[i].dst_rect.x < player[i].dst_rect.x)
        {
          enemy[i].move_angle = 90;
        }
        if (enemy[i].dst_rect.y > player[i].dst_rect.y)
        {
          enemy[i].move_angle = 0;
        }
        if (enemy[i].dst_rect.y < player[i].dst_rect.y)
        {
          enemy[i].move_angle = 180;
        }
        break;
      */
      case MT_RANDOM_AND_TRACKING:
        //プレイヤーとNPCとの距離が一定の距離より近い　かつ　same_place_flag == 0の時(追跡する)
        //printf("%d\n", enemy[i].move_angle);
        //プレイヤー3人の中で一番NPCとの距離が近いプレイヤーを求める
        min_distance = sqrt(pow(enemy[i].dst_rect.x - player[0].dst_rect.x, 2) + pow(enemy[i].dst_rect.y - player[0].dst_rect.y, 2));
        min_x = player[0].dst_rect.x;
        min_y = player[0].dst_rect.y;
        min_k = 0;
        if (min_distance > sqrt(pow(enemy[i].dst_rect.x - player[1].dst_rect.x, 2) + pow(enemy[i].dst_rect.y - player[1].dst_rect.y, 2)))
        {
          min_distance = sqrt(pow(enemy[i].dst_rect.x - player[1].dst_rect.x, 2) + pow(enemy[i].dst_rect.y - player[1].dst_rect.y, 2));
          min_x = player[1].dst_rect.x;
          min_y = player[1].dst_rect.y;
          min_k = 1;
        }
        if (min_distance > sqrt(pow(enemy[i].dst_rect.x - player[2].dst_rect.x, 2) + pow(enemy[i].dst_rect.y - player[2].dst_rect.y, 2)))
        {
          min_distance = sqrt(pow(enemy[i].dst_rect.x - player[2].dst_rect.x, 2) + pow(enemy[i].dst_rect.y - player[2].dst_rect.y, 2));
          min_x = player[2].dst_rect.x;
          min_y = player[2].dst_rect.y;
          min_k = 2;
        }

        //if (sqrt(pow(enemy[i].dst_rect.x - player[min_k].dst_rect.x, 2) + pow(enemy[i].dst_rect.y - player[min_k].dst_rect.y, 2)) < 200 && same_place_flag == 0)
        if (min_distance < 400 && same_place_flag == 0)
        {
          //printf("%d\n", min_distance);
          if (enemy[i].dst_rect.x > player[min_k].dst_rect.x) //プレイヤーが左方向にいる時
          {
            enemy[i].move_angle = 270;
          }
          if (enemy[i].dst_rect.x < player[min_k].dst_rect.x) //プレイヤーが右方向にいる時
          {
            enemy[i].move_angle = 90;
          }
          if (enemy[i].dst_rect.y > player[min_k].dst_rect.y) //プレイヤーが上方向にいる時
          {
            enemy[i].move_angle = 0;
          }
          if (enemy[i].dst_rect.y < player[min_k].dst_rect.y) //プレイヤーが下方向にいる時
          {
            enemy[i].move_angle = 180;
          }

          if (enemy[i].dst_rect.x == player[min_k].dst_rect.x && enemy[i].dst_rect.y == player[min_k].dst_rect.y)
          {
            enemy[i].move_angle = 360;
          }
          /*
            if (enemy[i].dst_rect.x == min_x && enemy[i].dst_rect.y == min_y)
            {
              enemy[i].move_angle = 360; //プレイヤーとx,y座標が等しい時、動かない
            }
            if (k != min_k)
            {
              enemy[i].move_angle = 360; //プレイヤーとx,y座標が等しい時、動かない
            }
            */
        }
        //プレイヤーとNPCとの距離が一定の距離より遠い時(ランダムウォークする)
        else
        {
          if (same_place_flag == 1 || enemy[i].prev_overlap_rect.w == 0 && enemy[i].prev_overlap_rect.h == 0)
          {
            if (random >= 0 && random <= 40)
            {
              enemy[i].move_angle = 0;
            }
            else if (random > 40 && random <= 50)
            {
              enemy[i].move_angle = 90;
            }
            else if (random > 50 && random <= 70)
            {
              enemy[i].move_angle = 180;
            }
            else if (random > 70 && random <= 100)
            {
              enemy[i].move_angle = 270;
            }
            else
            {
              printf("random 値エラー\n");
            }
            enemy[i].prev_overlap_rect = enemy[i].dst_rect; // 前回ランダムに方向転換した時の座標を保存しておく（同じ床で判定して無限ループにならないように）
          }
        }
      }
      break;
    }

    // 敵が1マス分動いてたら、前回の何かの床に乗った情報をリセット
    if (enemy[i].movetype != MT_RANDOM_AND_TRACKING)
    {
      if (abs(enemy[i].prev_overlap_rect.x - enemy[i].dst_rect.x) >= MAP_CHIPSIZE || abs(enemy[i].prev_overlap_rect.y - enemy[i].dst_rect.y) >= MAP_CHIPSIZE)
      {
        enemy[i].prev_overlap_rect.w = 0;
        enemy[i].prev_overlap_rect.h = 0;
      }
    }
    else if (abs(enemy[i].prev_overlap_rect.x - enemy[i].dst_rect.x) >= 20 || abs(enemy[i].prev_overlap_rect.y - enemy[i].dst_rect.y) >= 20) // 縦横20px進んだら、ランダムな方向に動く
    {
      enemy[i].prev_overlap_rect.w = 0;
      enemy[i].prev_overlap_rect.h = 0;
    }
    // 敵が画面外に行こうとしたときに向きを反転する
    if (enemy[i].dst_rect.x + enemy[i].dst_rect.w >= WINDOWWIDTH || enemy[i].dst_rect.x <= 0 || enemy[i].dst_rect.y + enemy[i].dst_rect.h >= WINDOWHEIGHT || enemy[i].dst_rect.y <= 0)
    {
      enemy[i].move_angle += 180;
      if (enemy[i].move_angle >= 360)
        enemy[i].move_angle -= 360;
    }

    int startTime, temp;
    // 催涙スプレーと、敵の判定
    for (int j = 0; j < PLAYER_NUM; j++)
    {
      // 催涙スプレーの当たり判定
      if (SDL_IntersectRectAndLine(&enemy[i].dst_rect, &player[j].spray_hitlines[0][0], &player[j].spray_hitlines[1][0], &player[j].spray_hitlines[0][1], &player[j].spray_hitlines[1][1]) ||
          SDL_IntersectRectAndLine(&enemy[i].dst_rect, &player[j].spray_hitlines[0][0], &player[j].spray_hitlines[1][0], &player[j].spray_hitlines[0][2], &player[j].spray_hitlines[1][2]) ||
          SDL_IntersectRectAndLine(&enemy[i].dst_rect, &player[j].spray_hitlines[0][0], &player[j].spray_hitlines[1][0], &player[j].spray_hitlines[0][3], &player[j].spray_hitlines[1][3]))
      {
        enemy[i].savetime = SDL_GetTicks(); // 催涙スプレーを当てた時間を保存
        enemy[i].speed = 0;                 // 敵のスピードを0にする
        enemy[i].flag_sairui = true;        // 催涙スプレーフラグを立てる
      }
    }
    // 催涙スプレーを当てた時間からSAIRUI_TIME立ったら元に戻る
    if (SDL_GetTicks() - enemy[i].savetime > SAIRUI_TIME && enemy[i].flag_sairui == true)
    {
      enemy[i].speed = ENEMY_SPEED; // 敵のスピードをもとに戻す
      enemy[i].flag_sairui = false; // 催涙スプレーフラグを下ろす
    }

    //動く方向を格納してる変数（move_angle）にしたがって進んでいく
    switch (enemy[i].move_angle)
    {
    case 0:
      enemy[i].dst_rect.y -= enemy[i].speed;
      break;
    case 90:
      enemy[i].dst_rect.x += enemy[i].speed;
      break;
    case 180:
      enemy[i].dst_rect.y += enemy[i].speed;
      break;
    case 270:
      enemy[i].dst_rect.x -= enemy[i].speed;
      break;
    case 360: //何もしない
      break;
    default:
      break; // その場で待機
    }
    // 敵と棚との衝突判定、敵のmovetypeによって処理を分ける
    for (int j = 0; j < kotei_object_num; j++)
    {
      SDL_Rect overrap_rect;

      // 敵が棚に当たった時、反転する movetypeが移動床または、ランダムのときのみ
      if (enemy[i].movetype == MT_MOVING_FLOOR || enemy[i].movetype == MT_RANDOM || enemy[i].movetype == MT_RANDOM_AND_TRACKING)
      {
        if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
        {
          switch (enemy[i].move_angle)
          {
          case 0:                                  //プレイヤーが上方向にいる　かつ　上方向に棚がある
            enemy[i].dst_rect.y += enemy[i].speed; //めり込みを戻す
            if (enemy[i].dst_rect.x > player[min_k].dst_rect.x)
            { //プレイヤーが左方向にいる時
              enemy[i].dst_rect.x -= enemy[i].speed;
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.x += enemy[i].speed;
              }
              //enemy[i].move_angle = 270;
            }
            else if (enemy[i].dst_rect.x < player[min_k].dst_rect.x)
            { //プレイヤーが右方向にいる時
              enemy[i].dst_rect.x += enemy[i].speed;
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.x -= enemy[i].speed;
              }
              //enemy[i].move_angle = 90;
            }
            break;
          case 90:                                 //プレイヤーが右方向にいる　かつ　右方向に棚がある
            enemy[i].dst_rect.x -= enemy[i].speed; //めり込みを戻す
            if (enemy[i].dst_rect.y > player[min_k].dst_rect.y)
            { //プレイヤーが上方向にいる時
              enemy[i].dst_rect.y -= enemy[i].speed;
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.y += enemy[i].speed;
              }
              //enemy[i].move_angle = 180;
            }
            else if (enemy[i].dst_rect.y < player[min_k].dst_rect.y)
            { //プレイヤーが下方向にいる時
              enemy[i].dst_rect.y += enemy[i].speed;
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.y -= enemy[i].speed;
              }
              //enemy[i].move_angle = 0;
            }
            break;
          case 180:
            enemy[i].dst_rect.y -= enemy[i].speed; //めり込みを戻す
            if (enemy[i].dst_rect.x > player[min_k].dst_rect.x)
            {
              enemy[i].dst_rect.x -= enemy[i].speed;
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.x += enemy[i].speed;
              }
              //enemy[i].move_angle = 270;
            }
            else if (enemy[i].dst_rect.x < player[min_k].dst_rect.x)
            {
              enemy[i].dst_rect.x += enemy[i].speed;
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.x -= enemy[i].speed;
              }
              //enemy[i].move_angle = 90;
            }
            break;
          case 270:
            enemy[i].dst_rect.x += enemy[i].speed; //めり込みを戻す
            if (enemy[i].dst_rect.y > player[min_k].dst_rect.y)
            {                                        //NPCが下、プレイヤーが上のとき
              enemy[i].dst_rect.y -= enemy[i].speed; //NPCを上に移動
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.y += enemy[i].speed;
              }
              //enemy[i].move_angle = 180;
            }
            else if (enemy[i].dst_rect.y < player[min_k].dst_rect.y)
            {                                        //NPCが上、プレイヤーが下のとき
              enemy[i].dst_rect.y += enemy[i].speed; //NPCを下に移動
              if (SDL_HasIntersection(&kotei_objects[j].dst_rect, &enemy[i].dst_rect) && kotei_objects[j].type == TYPE_SHELF)
              {
                enemy[i].dst_rect.y -= enemy[i].speed;
              }
              //enemy[i].move_angle = 0;
            }
            break;
          }
          enemy[i].move_angle += 180;
          if (enemy[i].move_angle >= 360)
            enemy[i].move_angle -= 360;
        }
      }
    }

    //printf("dest_rect.x = %d, before_enemy_x = %d\n", enemy[i].dst_rect.x, before_enemy_x);
    //printf("dest_rect.y = %d, before_enemy_y = %d\n", enemy[i].dst_rect.y, before_enemy_y);

    //if (!(sqrt(pow(enemy[i].dst_rect.x - before_enemy_x, 2))) == 0 && sqrt(pow(enemy[i].dst_rect.y - before_enemy_y, 2)) == 0)
    //現在のNPCのx座標と1つ前のNPCのx座標が異なる　かつ　現在のNPCのy座標と1つ前のNPCのy座標が異なる時
    if (enemy[i].dst_rect.x != before_enemy_x || enemy[i].dst_rect.y != before_enemy_y)
    {
      stay_start = SDL_GetTicks(); //留まっている時間カウントの開始時間を更新
    }
    stay_time = SDL_GetTicks() - stay_start; //留まっている時間
    //printf("stay_time = %d\n", stay_time);
    //printf("same_place_flag= %d\n", same_place_flag);
    if (stay_time >= 2000 && random_start_flag == 0) //留まっている時間が2秒以上の時
    {
      same_place_flag = 1;           //同じ座標に一定時間留まっていることを表すフラグを上げる
      random_start = SDL_GetTicks(); //ランダムウォーク時間カウントの開始時間を設定
      random_start_flag = 1;
    }
    random_time = SDL_GetTicks() - random_start; //ランダムウォークをしている時間
    //printf("random_time = %d\n", random_time);
    if (same_place_flag == 1 && random_time >= 3000)
    {
      same_place_flag = 0;   //same_place_flagを下ろす
      random_start_flag = 0; //random_start_flagを下ろす
    }
    before_enemy_x = enemy[i].dst_rect.x; //1つ前の座標を格納(x)
    before_enemy_y = enemy[i].dst_rect.y; //1つ前の座標を格納(y)

    // ゆっくり振り向く,最短で90度振り向いてほしいけど270度回ってしまう
    if (enemy[i].prev_angle != enemy[i].move_angle)
    {
      enemy[i].prev_angle += 3;
      if (enemy[i].prev_angle >= 360)
        enemy[i].prev_angle -= 360;

      // int diff = enemy[i].move_angle - enemy[i].prev_angle;
      // if(abs(diff) > 180){ diff -= 180; diff *= -1;}
      // if(diff < 0) {
      //   enemy[i].prev_angle += 10;
      // }
      // else if(diff > 0) {
      //   enemy[i].prev_angle -= 10;
      // }
      // if(enemy[i].prev_angle >= 360) enemy[i].prev_angle -= 360;
      // else if(enemy[i].prev_angle < 0) enemy[i].prev_angle += 360;
    }
  }
}


//敵が移動床に乗った時に、移動する方向を変える関数
int ChangeEnemyMoveAngle(enemyinfo *e, SDL_Rect movefloor, objecttype type)
{
  SDL_Rect adjusted_rect = e->dst_rect;
  adjusted_rect.x = movefloor.x + movefloor.w / 2 - e->dst_rect.w / 2;
  adjusted_rect.y = movefloor.y + movefloor.h / 2 - e->dst_rect.h / 2;
  e->dst_rect = adjusted_rect;
  switch (type)
  {
    case TYPE_ENEMY_MOVING_FLOOR_UL:
      if (e->move_angle == 90)
        e->move_angle = 0;
      if (e->move_angle == 180)
        e->move_angle = 270;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_UR:
      if (e->move_angle == 270)
        e->move_angle = 0;
      if (e->move_angle == 180)
        e->move_angle = 90;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_DL:
      if (e->move_angle == 90)
        e->move_angle = 180;
      if (e->move_angle == 0)
        e->move_angle = 270;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_DR:
      if (e->move_angle == 270)
        e->move_angle = 180;
      if (e->move_angle == 0)
        e->move_angle = 90;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_REV:
      e->move_angle += 180;
      if (e->move_angle >= 360)
        e->move_angle -= 360;
      break;
  }
}


//xとyと角度を与えると回転後の座標を返す関数
float Rotation(int x1, int y1, int a, int b, double theta, int *x2, int *y2)
{
  *x2 = (x1 - a) * cos(theta * M_PI / 180.0) - (y1 - b) * sin(theta * M_PI / 180.0) + a;
  *y2 = (x1 - a) * sin(theta * M_PI / 180.0) + (y1 - b) * cos(theta * M_PI / 180.0) + b;
}

void SetCamera()
{                         //カメラの初期値セット
  camera[0].angle = 30.0; //初期の回転位置
  camera[1].angle = 70.0;
  camera[2].angle = 180.0;
  camera[3].angle = 280.0;
  camera[4].angle = 80.0;
  for (int i = 0; i < CAMERA_NUM; i++)
  {
    camera[i].clockwise = true;
    Rotation(camera_dst_rects[i].x + camera_dst_rects[i].w, //回転後の座標を計算して返す
             camera_dst_rects[i].y + camera_dst_rects[i].h / 2,
             camera_dst_rects[i].x + camera_dst_rects[i].w / 2,
             camera_dst_rects[i].y + camera_dst_rects[i].h / 2,
             camera[i].angle,
             &camera[i].tri[0][0],
             &camera[i].tri[1][0]);
    //printf("x2 %d\n", camera[i].tri[0][0]);
    //printf("y2 %d\n", camera[i].tri[1][0]);
    /*
       camera[i].theta[0] = 90.0 - camera[i].angle;
       camera[i].theta[1] = 120.0 - camera[i].angle;
     */
    camera[i].theta[2] = 105 - camera[i].angle;
  }
}

void DrawMenu()
{
  CONTAINER data;
  memset(&data, 0, sizeof(CONTAINER)); //dataの初期化

  SDL_RenderCopy(mainrenderer, background[0].image_texture, &background[0].src_rect, &background[0].dst_rect); //背景をレンダーに出力
  for (int i = 0; i < FONT_NUM; i++)
  {
    boxColor(mainrenderer, font[i].dst_rect.x, font[i].dst_rect.y, font[i].dst_rect.x + font[i].dst_rect.w, font[i].dst_rect.y + font[i].dst_rect.h, 0xff000000);
    SDL_RenderCopy(mainrenderer, font[i].image_texture, &font[i].src_rect, &font[i].dst_rect); //フォントをレンダーに出力
  }
  if (player[myid].key.up == 1)
  {
    up = true;
    down = false;
  }
  else if (player[myid].key.down == 1)
  {
    up = false;
    down = true;
  }
  if (up)
  {
    rectangleColor(mainrenderer, font[0].dst_rect.x, font[0].dst_rect.y, font[0].dst_rect.x + font[0].dst_rect.w, font[0].dst_rect.y + font[0].dst_rect.h, 0xff00ff00);
    if (player[myid].key.a == 1)
    {                                      //スタートボタンが押された時
      data.command = START_COMMAND;        //コマンドを格納
      data.cid = myid;                     //クライアントIDを格納
      send_data(&data, sizeof(CONTAINER)); //クライアントのデータを送信
      player[myid].key.a = 0;
    }
  }
  else if (down)
  {
    rectangleColor(mainrenderer, font[1].dst_rect.x, font[1].dst_rect.y, font[1].dst_rect.x + font[1].dst_rect.w, font[1].dst_rect.y + font[1].dst_rect.h, 0xff00ff00);
    if (player[myid].key.a == 1)
      run = false;
  }
  SDL_RenderPresent(mainrenderer); // 描画データを表示
}

void Fontload()
{
  SDL_Surface *s; // サーフェイスを一時的に保存する変数
  SDL_Color black = {0, 0, 0, 255};
  SDL_Color white = {255, 255, 255, 255};
  SDL_Color red = {255, 0, 0, 255};
  //int iw,ih;
  for (int i = 0; i < FONT_NUM; i++)
  { //フォントロード
    s = TTF_RenderUTF8_Blended(japanesefont, fonts[i], white);
    font[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);

    font[i].src_rect.x = 0;
    font[i].src_rect.y = 0;
    font[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    font[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    font[i].dst_rect = font_dst_rects[i];
    font[i].dst_rect.w = s->w;
    font[i].dst_rect.h = s->h;
  }
  printf("フォントロード2終了\n");
}

void MakeMap()
{
  /* マップの読み込みと配置 */
  int i, j, index = 0, enemy_index = 0, player_index = 0, loadmap_objecttype;
  SDL_Surface *s;
  SDL_Rect src = {0, 0, MAP_CHIPSIZE, MAP_CHIPSIZE};
  SDL_Rect dst = {0};

  // マップデータをfor文で1マスずつ読み込んでいく
  for (j = 0; j < MAP_HEIGHT; j++, dst.y += MAP_CHIPSIZE)
  {

    dst.x = 0;
    for (i = 0; i < MAP_WIDTH; i++, dst.x += MAP_CHIPSIZE)
    {
      loadmap_objecttype = map0[j][i];      // マップデータを格納する
      if (loadmap_objecttype == TYPE_ENEMY) // 読み込んだマップデータが敵のとき
      {
        //構造体enemyに、敵の情報を格納

        enemy_index = InitObjectFromMap(enemy_index, loadmap_objecttype, dst);
      }
      else if (loadmap_objecttype == TYPE_PLAYER1 || loadmap_objecttype == TYPE_PLAYER2 || loadmap_objecttype == TYPE_PLAYER3)
      { // 読み込んだマップデータがプレイヤーのとき
        //構造体playerに、プレイヤーの情報を格納
        player_index = InitObjectFromMap(player_index, loadmap_objecttype, dst);
      }
      else if ((loadmap_objecttype >= TYPE_KINKAI && loadmap_objecttype <= TYPE_ENTRANCE) || (loadmap_objecttype >= TYPE_ENEMY_MOVING_FLOOR_UL && loadmap_objecttype <= TYPE_ENEMY_MOVING_FLOOR_REV))
      {
        // 棚、出入り口、金塊の情報をkotei_objectに格納
        index = InitObjectFromMap(index, loadmap_objecttype, dst);
      }
    }
    kotei_object_num = index;
  }
}

void setup_client(char *server_name, u_short port)
{
  struct hostent *server; //インターネットホスト名からアドレスへの割り当て
  struct sockaddr_in sv_addr;

  fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
  /* gethostbyname関数
     呼び出しで指定されたホスト名用の hostent 構造体へのポインターを戻す*/
  if ((server = gethostbyname(server_name)) == NULL)
  {
    handle_error("gethostbyname()");
  }

  //ソケットの生成(クライアント側)
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  { //正常に実行されなかった場合
    handle_error("socket()");
  }

  sv_addr.sin_family = AF_INET;   //ネットワークアドレスの種類(ここでは、インターネット)
  sv_addr.sin_port = htons(port); //ポート番号
  sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0];

  if (connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
  { //クライアントからサーバへの接続要求
    handle_error("connect()");
  }

  fprintf(stderr, "Input your name: ");
  char user_name[MAX_LEN_NAME]; //ユーザ名格納(最大10文字)
  /*
     ・fgets(user_name, sizeof(user_name), stdin)について
     fgets関数は、改行までの文字列をまとめて読み込むための関数であり、改行コードも格納される
     stdinと指定しているので、標準入力である。
   */
  if (fgets(user_name, sizeof(user_name), stdin) == NULL)
  { //ユーザ名の取得
    handle_error("fgets()");
  }
  //user_name[]の最後尾に含まれる改行コードをヌル文字に変更
  user_name[strlen(user_name) - 1] = '\0';
  send_data(user_name, MAX_LEN_NAME); //ユーザ名をサーバに送信

  fprintf(stderr, "Waiting for other clients...\n");
  receive_data(&num_clients, sizeof(int)); //サーバに接続されているクライアントの数の読み込み
  fprintf(stderr, "Number of clients = %d.\n", num_clients);
  receive_data(&myid, sizeof(int)); //クライアント自身の識別IDを読み込み
  fprintf(stderr, "Your ID = %d.\n", myid);
  int i;
  for (i = 0; i < num_clients; i++)
  { //全クライアントのデータの受信
    receive_data(&clients[i], sizeof(CLIENT));
  }

  num_sock = sock + 1;
  FD_ZERO(&mask);      //&maskをゼロクリア
  FD_SET(0, &mask);    //0番目のFDに対応する値を1にセット
  FD_SET(sock, &mask); //sock番目のFDに対応する値を1にセット
  fprintf(stderr, "Input command (M=message, Q=quit): \n");
}

int control_requests()
{
  fd_set read_flag = mask;

  struct timeval timeout; //タイマ値を指定する
  timeout.tv_sec = 0;     //秒単位
  timeout.tv_usec = 30;   //マイクロ秒単位

  if (select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1)
  {
    handle_error("select()");
  }

  int result = 1;
  /*
     FD集合&read_flagの、0番目のビットが立っているかどうか
     立っていれば、0 以外の値を返し、存在しなければ 0 を返す
   */

  /*
  //クライアント自身がコマンドを入力したとき
  if (FD_ISSET(0, &read_flag))
  {
  result = input_command(); //サーバからコマンドが送信されてきたとき
  }
   */
  if (FD_ISSET(sock, &read_flag))
  {
    result = execute_command();
  }

  return result;
  // マップはテクスチャに
  // if (NULL == (gGame.map = SDL_CreateTextureFromSurface(gGame.render, map)))
  // {
  //   ret = PrintError(SDL_GetError());
  // }
}

void joystick_send(int num) //ジョイスティックの操作に関する情報を送信する関数
{
  CONTAINER data;
  memset(&data, 0, sizeof(CONTAINER)); //dataの初期化
  if (num == 0)                        //座標の情報を送信
  {
    //コマンドとして、座標の'Z'を代入
    data.command = ZAHYO_COMMAND;           //コマンドを格納
    data.cid = myid;                        //クライアントIDを格納
    data.zahyo_x = player[myid].dst_rect.x; //プレイヤーのx座標を格納
    data.zahyo_y = player[myid].dst_rect.y; //プレイヤーのy座標を格納
  }
  else if(num == 13){
    data.command = ENEMY_MODIFY_COMMAND;           //コマンドを格納
    data.cid = myid;                        //クライアントIDを格納
    data.enemy_zahyo_x = enemy[0].dst_rect.x; //NPCのx座標
    data.enemy_zahyo_y = enemy[0].dst_rect.y; //NPCのy座標
    data.move_angle = enemy[0].move_angle;
    data.prev_angle = enemy[0].prev_angle;
  }
  else if (num == 1) //金塊の設置の可否を送信
  {
    //コマンドとして、座標の'K'を代入
    data.command = KINKAI_COMMAND; //コマンドを格納
    data.cid = myid;               //クライアントIDを格納
  }
  else if (num == 2)
  {
    //コマンドとして、プレイヤーの'P'を代入
    data.command = PLAYER_COMMAND; //コマンドを格納
    data.cid = myid;               //クライアントIDを格納
  }
  else if (num == 3)
  { //右のスティック操作
    //コマンドとして、プレイヤーの'R'を代入
    data.command = RIGHT_COMMAND; //コマンドを格納
    data.cid = myid;              //クライアントIDを格納
  }
  else if (num == 4)
  { //左のスティック操作
    //コマンドとして、プレイヤーの'L'を代入
    data.command = LEFT_COMMAND; //コマンドを格納
    data.cid = myid;             //クライアントIDを格納
  }
  else if (num == 5)
  { //上のスティック操作
    //コマンドとして、プレイヤーの'U'を代入
    data.command = UP_COMMAND; //コマンドを格納
    data.cid = myid;           //クライアントIDを格納
  }
  else if (num == 6)
  { //下のスティック操作
    //コマンドとして、プレイヤーの'D'を代入
    data.command = DOWN_COMMAND; //コマンドを格納
    data.cid = myid;             //クライアントIDを格納
  }
  else if (num == 7)
  { //真ん中(左右)のスティック操作
    //コマンドとして、プレイヤーの'C'を代入
    data.command = CENTER_COMMAND; //コマンドを格納
    data.cid = myid;               //クライアントIDを格納
  }
  else if (num == 8)
  { //真ん中(上下)のスティック操作
    //コマンドとして、プレイヤーの'A'を代入
    data.command = AENTER_COMMAND; //コマンドを格納
    data.cid = myid;               //クライアントIDを格納
  }
  else if (num == 9) //ハッキング
  {
    printf("joystickhack\n");
    data.command = HACK_COMMAND;
    data.cid = myid;
  }
  else if (num == 10) //ハッキングチャージ
  {
    printf("hackstart\n");
    data.command = HACK_START_COMMAND;
    data.cid = myid;
  }
  else if (num == 11) //ハッキングキャンセル
  {
    printf("joystickhack\n");
    data.command = NOT_HACK_COMMAND;
    data.cid = myid;
  }

  else if (num == 12)
  { //3ボタンを押した時
    //コマンドとして、2ボタンをスーファミに見立てたときの'X'を代入
    data.command = X_ON_COMMAND; //コマンドを格納
    data.cid = myid;             //クライアントIDを格納
  }
  else if (num == 14)
  { //3ボタンを離した時
    //コマンドとして、2ボタンをスーファミに見立てたときの'X'を代入
    data.command = X_OFF_COMMAND; //コマンドを格納
    data.cid = myid;              //クライアントIDを格納
  }

  send_data(&data, sizeof(CONTAINER)); //クライアントのデータを送信
  fprintf(stderr, "send_data %d\n", num);
}
static int input_command()
{ //クライアントがデータをインプットした時
  CONTAINER data;
  char com;
  memset(&data, 0, sizeof(CONTAINER)); //dataの初期化
  com = getchar();                     //標準入力から１文字を受け取る('M'または'Q')
  while (getchar() != '\n')
    ; //改行コードが入力されるまで，1文字ずつ文字を受けとる

  switch (com)
  {
    case MESSAGE_COMMAND: //'M'のとき
      fprintf(stderr, "Input message: ");
      if (fgets(data.message, MAX_LEN_BUFFER, stdin) == NULL)
      { //メッセージの受け取り
        handle_error("fgets()");
      }
      data.command = MESSAGE_COMMAND;                //コマンドを格納
      data.message[strlen(data.message) - 1] = '\0'; //メッセージの最後にヌル文字を代入
      data.cid = myid;                               //クライアントIDを格納
      send_data(&data, sizeof(CONTAINER));           //クライアントのデータを送信
      break;
    case QUIT_COMMAND:                     //'Q'のとき
      data.command = QUIT_COMMAND;         //コマンドを格納
      data.cid = myid;                     //クライアントIDを格納
      send_data(&data, sizeof(CONTAINER)); //クライアントのデータを送信
      break;
    default: //その他の文字が入力された場合
      fprintf(stderr, "%c is not a valid command.\n", com);
  }

  return 1;
}

static int execute_command()
{ //サーバからデータを受け取ったとき
  CONTAINER data;
  int result = 1;
  memset(&data, 0, sizeof(CONTAINER)); //dataの初期化
  receive_data(&data, sizeof(data));   //クライアントのデータを受信

  switch (data.command)
  {

  case ZAHYO_COMMAND: //'Z'のとき
    //自分の座標は、スティックを動かした段階で更新してるので、ここでは、更新せず。
    //if (myid != data.cid)
    //{
      player[data.cid].dst_rect.x = data.zahyo_x; //クライアントのx座標を各プレイヤーの座標を反映
      player[data.cid].dst_rect.y = data.zahyo_y; //クライアントのy座標を各プレイヤーの座標を反映
      player[data.cid].back_zahyo_x = data.zahyo_x;
      player[data.cid].back_zahyo_y = data.zahyo_y;
    //}
    //fprintf(stderr, "client[%d], name : %s,zahyo_x = %d, zahyo_y = %d \n", data.cid, clients[data.cid].name, data.zahyo_x, data.zahyo_y);
    result = 1;
    break;
    case KINKAI_COMMAND: //'K'のとき
      fprintf(stderr, "client[%d], name : %s, get kinkai !!!!! \n", data.cid, clients[data.cid].name);
      kinkai_flag = false;
      if (data.cid == myid)
      { //金塊を取った、クライアントのIDが自分のIDと同じであれば
        kinkai_keep_flag = true;
        player[myid].flag_kinkai = true;
      }
      result = 1;
      break;
    case HACK_COMMAND: //'H'のとき
      printf("executehack\n");
      time_now = SDL_GetTicks();
      hacking_flag = true;
      player[data.cid].flag_hack_start = false;
      player[data.cid].key.y= 0;
      player[data.cid].hack = 0;
      player[data.cid].speed= PLAYER_SPEED;
      result = 1;
      break;
    case NOT_HACK_COMMAND: //'N'のとき(長押ししたけどキャンセルしたとき)
      printf("cancelhack\n");
      player[data.cid].flag_hack_start = false;
      player[data.cid].key.y= 0;
      player[data.cid].speed= PLAYER_SPEED;
      result = 1;
      break;
    case HACK_START_COMMAND: //'V'のとき
      player[data.cid].inputtime = SDL_GetTicks();
      player[data.cid].flag_hack_start = true;
      player[data.cid].key.y= 1;
      player[data.cid].speed= 0;
      gauge = 0;
      printf("executestarthack\n");
      result = 1;
      break;
  case PLAYER_COMMAND: //'P'のとき
    if (myid != data.cid)
    {
      player_flag[data.cid] = false; //他のクライアントから消えたと通知がきたプレイヤーを描画しないようにする
    }
    //fprintf(stderr, "client[%d], name : %s, get kinkai !!!!! \n", data.cid, clients[data.cid].name);
    //kinkai_flag = false;
    result = 1;
    break;
  case RIGHT_COMMAND:               //'R'のとき
    player[data.cid].key.right = 1; //スティックが右に入っていることを維持
    player[data.cid].key.left = 0;
    result = 1;
    break;
  case LEFT_COMMAND: //'L'のとき
    player[data.cid].key.right = 0;
    player[data.cid].key.left = 1; //スティックが右に入っていることを維持
    result = 1;
    break;
  case UP_COMMAND:               //'U'のとき
    player[data.cid].key.up = 1; //スティックが上に入っていることを維持
    player[data.cid].key.down = 0;
    result = 1;
    break;
  case DOWN_COMMAND: //'D'のとき
    player[data.cid].key.up = 0;
    player[data.cid].key.down = 1; //スティックが右に入っていることを維持
    result = 1;
    break;
  case CENTER_COMMAND: //'C'のとき
    player[data.cid].key.right = 0;
    player[data.cid].key.left = 0;
    result = 1;
    break;
  case AENTER_COMMAND: //'A'のとき
    player[data.cid].key.up = 0;
    player[data.cid].key.down = 0;
    result = 1;
    break;
  case MESSAGE_COMMAND: //'M'のとき
    fprintf(stderr, "client[%d] %s: %s\n", data.cid, clients[data.cid].name, data.message);
    result = 1;
    break;
  case START_COMMAND: //'S'のとき
    fprintf(stderr, "client[%d] %s: %s\n", data.cid, clients[data.cid].name, data.message);
    status = GAMEMODE;
    result = 1;
    stay_start = SDL_GetTicks();
  case QUIT_COMMAND: //'Q'のとき
    fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
    result = 0;
    break;
  case X_ON_COMMAND:            //'X'のとき
    player[data.cid].key.x = 1; //2ボタンが入力されていることを維持
    result = 1;
    break;
  case X_OFF_COMMAND:           //'Y'のとき
    player[data.cid].key.x = 0; //2ボタンが離されていることを維持
    result = 1;
    break;
  case ENEMY_MODIFY_COMMAND:
    enemy[0].dst_rect.x = data.enemy_zahyo_x; //NPCのx座標
    enemy[0].dst_rect.y = data.enemy_zahyo_y; //NPCのy座標
    enemy[0].move_angle = data.move_angle;
    enemy[0].prev_angle = data.prev_angle;
    result = 1;
    break;
  default: //その他の文字が入力された場合
    //fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);
    //exit(1); //異常終了
    break;
  }
  return result;
}

static void send_data(void *data, int size)
{
  if ((data == NULL) || (size <= 0))
  { //CONTAINER構造体で宣言されたdataに不具合がある場合
    fprintf(stderr, "send_data(): data is illeagal.\n");
    exit(1); //異常終了
  }
  //引数2が指すバッファーから、ファイルディスクリプター引数1が参照するファイルへ、最大引数3のバイト数分を書き込む
  if (write(sock, data, size) == -1)
  {
    handle_error("write()");
  }
}

static int receive_data(void *data, int size)
{
  if ((data == NULL) || (size <= 0))
  { //CONTAINER構造体で宣言されたdataに不具合がある場合
    fprintf(stderr, "receive_data(): data is illeagal.\n");
    exit(1); //異常終了
  }

  return (read(sock, data, size)); //コネクションからバッファへのデータの読み込み
}

static void handle_error(char *message)
{
  perror(message); //システム・エラー・メッセージを伴うエラー・メッセージ
  /*
errno : システムコールや一部のライブラリ関数の実行に失敗した際、
どのような原因で失敗したのかを教えてくれる
   */
  fprintf(stderr, "%d\n", errno);
  exit(1); //エラー終了
}

void terminate_client()
{
  fprintf(stderr, "Connection is closed.\n");
  close(sock); ////ソケットを切断
  exit(0);     //正常終了
}

int InitObjectFromMap(int index, objecttype loadmap_objecttype, SDL_Rect dst)
{
  SDL_Surface *s;

  if (loadmap_objecttype == TYPE_ENEMY)
  { // マップから読み込んだのが敵(敵)だったとき
    //構造体enemyに、敵の情報を格納
    enemy[index].type = TYPE_ENEMY;
    s = IMG_Load(imgfiles[TYPE_ENEMY]);
    if (s == NULL)
      fprintf(stderr, "Missing Open Surface: maptype %d", loadmap_objecttype);
    enemy[index].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    enemy[index].src_rect.x = 0;
    enemy[index].src_rect.y = 0;
    enemy[index].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    enemy[index].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定

    enemy[index].dst_rect.x = dst.x + ((MAP_CHIPSIZE - s->w) / 2); // マップで指定された場所 + MAP_CHIPSIZEの中心になるように足し算
    enemy[index].dst_rect.y = dst.y + ((MAP_CHIPSIZE - s->h) / 2);
    enemy[index].dst_rect.w = s->w; // ゲーム画面に描画される敵の画像の幅、高さは元画像のままにする
    enemy[index].dst_rect.h = s->h;
    enemy[index].speed = ENEMY_SPEED; // ヘッダで指定した定数をプレイヤーの移動スピードとして設定
    enemy[index].flag_sairui = false;
    enemy[index].look_angle = enemy_lookangles[index];
    enemy[index].move_angle = enemy_moveangles[index];
    enemy[index].prev_overlap_rect.x = 0;
    enemy[index].prev_overlap_rect.y = 0;
    enemy[index].prev_overlap_rect.w = 0;
    enemy[index].prev_overlap_rect.h = 0;
    enemy[index].movetype = enemy_movetypes[index];
    enemy[index].prev_angle = enemy_moveangles[index];
    index++;
  }
  else if (loadmap_objecttype == TYPE_PLAYER1 || loadmap_objecttype == TYPE_PLAYER2 || loadmap_objecttype == TYPE_PLAYER3)
  { // マップから読み込んだのがプレイヤーだったとき
    //構造体playerに、敵の情報を格納

    player[index].type = loadmap_objecttype;
    s = IMG_Load(imgfiles[loadmap_objecttype]);
    if (s == NULL)
      fprintf(stderr, "Missing Open Surface: maptype %d", loadmap_objecttype);
    player[index].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    player[index].src_rect.x = 0;
    player[index].src_rect.y = 0;
    player[index].src_rect.w = 24;                                // 読み込んだ画像ファイルの幅を元画像の領域として設定
    player[index].src_rect.h = 24;                                // 読み込んだ画像ファイルの高さを元画像の領域として設定
    player[index].dst_rect.x = dst.x + ((MAP_CHIPSIZE - 24) / 2); // マップで指定された場所 + MAP_CHIPSIZEの中心になるように足し算
    player[index].dst_rect.y = dst.y + ((MAP_CHIPSIZE - 24) / 2);
    player[index].dst_rect.w = 24; // ゲーム画面に描画される敵の画像の幅、高さは元画像のままにする
    player[index].dst_rect.h = 24;
    s = IMG_Load(imgfiles[TYPE_SPRAY]);
    if (s == NULL)
      fprintf(stderr, "Missing Open Surface: %s", imgfiles[TYPE_SPRAY]);
    player[index].spray_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    player[index].spray_src_rect.x = 0;
    player[index].spray_src_rect.y = 0;
    player[index].spray_src_rect.w = s->w;
    player[index].spray_src_rect.h = s->h;
    player[index].spray_dst_rect.w = SPRAY_WIDTH;
    player[index].spray_dst_rect.h = SPRAY_HEIGHT;
    for (int i = 0; i < 2; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        player[index].spray_hitlines[i][j] = 0;
      }
    }
    player[index].spray_origin.x = 0;
    player[index].spray_origin.y = SPRAY_HEIGHT / 2;
    player[index].back_zahyo_x = player[index].dst_rect.x; //プレイヤーの座標をfloat型で持つ(斜め移動の加速防止用)
    player[index].back_zahyo_y = player[index].dst_rect.y; //プレイヤーの座標をfloat型で持つ(斜め移動の加速防止用)
    player[index].flag_kinkai = false;
    player[index].speed = PLAYER_SPEED;                    // ヘッダで指定した定数をプレイヤーの移動スピードとして設定
    player[index].look_angle = 0;                          // プレイヤーの最初の見てる角度、0度に設定
    player[index].spray_flag = 0;
    player[index].spraytime = SPRAY_TIME;

    player[index].hack = 1;
    index++;
  }
  else if ((loadmap_objecttype >= TYPE_KINKAI && loadmap_objecttype <= TYPE_ENTRANCE) || (loadmap_objecttype >= TYPE_ENEMY_MOVING_FLOOR_UL && loadmap_objecttype <= TYPE_ENEMY_MOVING_FLOOR_REV))
  { // マップから読み込んだのが金塊、棚、出入り口、敵の移動床のとき

    kotei_objects[index].type = loadmap_objecttype;
    s = IMG_Load(imgfiles[loadmap_objecttype]);
    if (s == NULL)
      fprintf(stderr, "Missing Open Surface: maptype %d", loadmap_objecttype);
    kotei_objects[index].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    kotei_objects[index].src_rect.x = 0;
    kotei_objects[index].src_rect.y = 0;
    kotei_objects[index].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[index].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定

    kotei_objects[index].dst_rect.x = dst.x; // マップで指定された場所に出力されるように設定
    kotei_objects[index].dst_rect.y = dst.y;
    kotei_objects[index].dst_rect.w = MAP_CHIPSIZE; // 幅、高さはCHIPSIZEにする
    kotei_objects[index].dst_rect.h = MAP_CHIPSIZE;
    index++;
  }
  return index;
}

void PlayerAction()
{
  // 催涙スプレーを出す動作
  int origin_x, origin_y;
  for (int i = 0; i < PLAYER_NUM; i++)
  {
    if (player_flag[myid] == false)
      continue; // プレイヤーがいないとき、催涙スプレーを出さない
    // スプレーが切れたとき、出さない
    if (player[i].spraytime < 0)
    {
      player[i].spray_flag = false;
      for (int j = 0; j < 4; j++)
      {
        player[i].spray_hitlines[0][j] = 0;
        player[i].spray_hitlines[1][j] = 0;
      }
      continue;
    }
    if (player[i].key.x)
    {
      origin_x = player[i].dst_rect.x;
      origin_y = player[i].dst_rect.y;
      switch (player[i].look_angle)
      {
        case 0:
          origin_x += player[i].src_rect.w / 2;
          origin_y -= SPRAY_HEIGHT / 2;
          break;
        case 45:
          origin_x += player[i].src_rect.w;
          origin_y -= SPRAY_HEIGHT / 2;
          break;
        case 90:
          origin_x += player[i].src_rect.w;
          origin_y += player[i].src_rect.h / 2 - SPRAY_HEIGHT / 2;
          break;
        case 135:
          origin_x += player[i].src_rect.w;
          origin_y += player[i].src_rect.h - SPRAY_HEIGHT / 2;
          break;
        case 180:
          origin_x += player[i].src_rect.w / 2;
          origin_y += player[i].src_rect.h - SPRAY_HEIGHT / 2;
          break;
        case 225:
          origin_y += player[i].src_rect.h - SPRAY_HEIGHT / 2;
          break;
        case 270:
          origin_y += player[i].src_rect.h / 2 - SPRAY_HEIGHT / 2;
          break;
        case 315:
          origin_y -= SPRAY_HEIGHT / 2;
          break;
      }
      //催涙スプレーの当たり判定(線)を作る, 当たり判定の範囲はSPRAY_WIDTH,HEIGHTの定数で調節(現在は画像の幅と一緒に)
      player[i].spray_dst_rect.x = origin_x;
      player[i].spray_dst_rect.y = origin_y;
      player[i].spray_dst_rect.w = SPRAY_WIDTH;
      player[i].spray_dst_rect.h = SPRAY_HEIGHT;
      player[i].spray_hitlines[0][0] = origin_x;
      player[i].spray_hitlines[1][0] = origin_y + player[i].dst_rect.h;
      player[i].spray_hitlines[0][1] = origin_x + SPRAY_WIDTH * sin(player[i].look_angle * M_PI / 180);
      player[i].spray_hitlines[1][1] = origin_y + player[i].dst_rect.h + SPRAY_WIDTH * -cos((player[i].look_angle) * M_PI / 180);
      // 15度回転させた催涙スプレーの当たり判定を作る
      int look_angle_vision[2] = {player[i].look_angle - 15, player[i].look_angle + 15};
      for (int j = 0; j < 2; j++)
      {
        if (look_angle_vision[j] < 0)
          look_angle_vision[j] += 360;
        else if (look_angle_vision[j] >= 360)
          look_angle_vision[j] -= 360;
        player[i].spray_hitlines[0][j + 2] = origin_x + SPRAY_WIDTH * sin(look_angle_vision[j] * M_PI / 180);
        player[i].spray_hitlines[1][j + 2] = origin_y + player[i].dst_rect.h + SPRAY_WIDTH * -cos((look_angle_vision[j]) * M_PI / 180);
      }
      player[i].spraytime--;
      //催涙スプレーフラグを立てる
      player[i].spray_flag = 1;
    }
    // キー押されてないとき、催涙スプレーの当たり判定をリセットする
    else if (player[i].key.x == 0)
    {
      player[i].spray_flag = 0;
      for (int j = 0; j < 4; j++)
      {
        player[i].spray_hitlines[0][j] = 0;
        player[i].spray_hitlines[1][j] = 0;
      }
    }
  }
}

void Events() {
  //ハッキング関連
  if(hacking_flag){
    if(SDL_GetTicks() - time_now > STOPTIME) hacking_flag = false;
  }
}
