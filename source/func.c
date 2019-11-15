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

void RenderWindow(void) //画面の描画(イベントが無い時)
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer);       // 設定した描画色(白)でレンダラーをクリア
  for(int i=0; i<KOTEI_OBJECT_NUM; i++){
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); //固定オブジェクトをレンダーに出力
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
  int move_distance = PLAYER_SPEED;
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

  //敵キャラの移動
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    for(int j=0; j < KOTEI_OBJECT_NUM; j++){
      if(!(kotei_objects[j].type >= TYPE_ENEMY_MOVING_FLOOR_DL)) break;
      printf("aaa");
      SDL_Rect overrap_rect;
      if(SDL_IntersectRect(&(kotei_objects[j].dst_rect), &(enemy[i].dst_rect), &overrap_rect)){
      //&&
   //                       overrap_rect.w >= enemy[i].dst_rect.w &&
   //                       overrap_rect.h >= enemy[i].dst_rect.h){
                            printf("overlap\n");
                          //&&
//                          abs((enemy[i].dst_rect.x + enemy[i].dst_rect.w/2) - (kotei_objects[j].dst_rect.x + kotei_objects[j].dst_rect.w)) <= 4){ // 敵の移動床と、敵が完全に重なって、敵の座標が移動床の真ん中に近い（4px以内）のとき
        if(enemy[i].prev_overlap_rect.w == 0 || (abs(enemy[i].prev_overlap_rect.x - enemy[i].dst_rect.x) > 5 && abs(enemy[i].prev_overlap_rect.x - enemy[i].dst_rect.y)) > 5){
          ChangeEnemyMoveAngle(&enemy[i],kotei_objects[j].dst_rect,kotei_objects[j].type); // 敵の動く方向をかえる
          enemy[i].prev_overlap_rect = overrap_rect;
        }
      }
    }
    //動く方向を格納してる変数（move_angle）に進んでいく
    switch (enemy[i].move_angle)
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
  //棚との衝突判定
  for (int i = 0; i < KOTEI_OBJECT_NUM; i++)
  {
    if (SDL_HasIntersection(&(kotei_objects[i].dst_rect), &(player[0].dst_rect))) // プレイヤーと固定オブジェクトが重なった時
    {
      if (kotei_objects[i].type != TYPE_SHELF) // 棚以外とぶつかったときは無視
        break;

      // ぶつかったぶんの距離プレイヤーの位置を戻す
      if (key.left) player[0].dst_rect.x += move_distance;
      if (key.right) player[0].dst_rect.x -= move_distance;
      if (key.up) player[0].dst_rect.y += move_distance;
      if (key.down) player[0].dst_rect.y -= move_distance;
    }
  }
}

int ChangeEnemyMoveAngle(enemyinfo *e,SDL_Rect movefloor, objecttype type){
  SDL_Rect adjusted_rect = e->dst_rect;
  adjusted_rect.x = movefloor.x + movefloor.w / 2 - e->dst_rect.w/2;
  adjusted_rect.y = movefloor.y + movefloor.h / 2 - e->dst_rect.h/2;
  e->dst_rect = adjusted_rect;
  switch(type){
    case TYPE_ENEMY_MOVING_FLOOR_UR:
      if(e->move_angle == 270) e->move_angle = 0;
      if(e->move_angle == 180) e->move_angle = 90;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_UL:
      if(e->move_angle == 90) e->move_angle = 0;
      if(e->move_angle == 180) e->move_angle = 270;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_DL:
      if(e->move_angle == 90) e->move_angle = 180;
      if(e->move_angle == 0) e->move_angle = 270;
      break;
    case TYPE_ENEMY_MOVING_FLOOR_DR:
      if(e->move_angle == 270) e->move_angle = 180;
      if(e->move_angle == 0) e->move_angle = 90;
      break;
  }
}

void MakeMap()
{
  /* マップの読み込みと配置 */
  int i, j, index=0,enemy_index=0,player_index=0,loadmap_objecttype;
  SDL_Surface *s;
  SDL_Rect src = {0, 0, MAP_CHIPSIZE, MAP_CHIPSIZE};
  SDL_Rect dst = {0};

  // マップデータをfor文で1マスずつ読み込んでいく
  for (j = 0; j < MAP_HEIGHT; j++, dst.y += MAP_CHIPSIZE)
  {
    dst.x = 0;
    for (i = 0; i < MAP_WIDTH; i++, dst.x += MAP_CHIPSIZE)
    {
      loadmap_objecttype = map0[j][i]; // マップデータを格納する
      if(loadmap_objecttype == TYPE_NONE) break; // マップデータなしのとき、break
      if (loadmap_objecttype == TYPE_ENEMY) // 読み込んだマップデータが敵のとき
      {
        //構造体enemyに、敵の情報を格納
        enemy_index = InitObjectFromMap(enemy_index, loadmap_objecttype,dst);
      }
      else if(loadmap_objecttype == TYPE_PLAYER){ // 読み込んだマップデータがプレイヤーのとき
        //構造体playerに、プレイヤーの情報を格納
        player_index = InitObjectFromMap(player_index,loadmap_objecttype,dst);
      }
      else
      {
        // 棚、出入り口、金塊の情報をkotei_objectに格納
        index = InitObjectFromMap(index, loadmap_objecttype,dst);
      }
    }
  }
}

int InitObjectFromMap(int index, objecttype loadmap_objecttype, SDL_Rect dst)
{
  SDL_Surface *s;

  if(loadmap_objecttype == TYPE_ENEMY){
    //構造体enemyに、敵の情報を格納
    enemy[index].type = TYPE_ENEMY;
    s = IMG_Load(imgfiles[TYPE_ENEMY]);
    if (s == NULL) fprintf(stderr,"Missing Open Surface: maptype %d",loadmap_objecttype);
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
    enemy[index].isgodest = false;
    enemy[index].look_angle = enemy_lookangles[index];
    enemy[index].move_angle = enemy_moveangles[index];
    enemy[index].prev_overlap_rect.x = 0;
    enemy[index].prev_overlap_rect.y = 0;
    enemy[index].prev_overlap_rect.w = 0;
    enemy[index].prev_overlap_rect.h = 0;
    index++;
  }
  else if(loadmap_objecttype == TYPE_PLAYER){
    //構造体playerに、敵の情報を格納
    player[index].type = TYPE_PLAYER;
    s = IMG_Load(imgfiles[TYPE_PLAYER]);
    if (s == NULL) fprintf(stderr,"Missing Open Surface: maptype %d",loadmap_objecttype);
    player[index].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    player[index].src_rect.x = 0;
    player[index].src_rect.y = 0;
    player[index].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    player[index].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定

    player[index].dst_rect.x = dst.x + ((MAP_CHIPSIZE - s->w) / 2); // マップで指定された場所 + MAP_CHIPSIZEの中心になるように足し算
    player[index].dst_rect.y = dst.y + ((MAP_CHIPSIZE - s->h) / 2);
    player[index].dst_rect.w = s->w; // ゲーム画面に描画される敵の画像の幅、高さは元画像のままにする
    player[index].dst_rect.h = s->h;
    player[index].speed = PLAYER_SPEED; // ヘッダで指定した定数をプレイヤーの移動スピードとして設定

    index++;
  }
  else{
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
