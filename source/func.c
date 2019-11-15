#include "../common.h"
#include "../constants.h"
#include "func.h"

static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask; //FD集合を表す構造体
static CLIENT clients[MAX_NUM_CLIENTS];

void setup_client(char *, u_short);
int control_requests();
void terminate_client();
void joystick_send(int);

static int input_command(void);
static int execute_command(void);
static void send_data(void *, int);
static int receive_data(void *, int);
static void handle_error(char *);

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
  run = true; //動かす
  for (int i = 0; i < CAMERA_NUM; i++)
  {
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
  //kinkai_flag = true;                                   //金塊は最初は、配置されている
  player_flag[0] = true;                                //プレイヤー1 は最初は、生存
  player_flag[1] = true;                                //プレイヤー2 は最初は、生存
  player_flag[2] = true;                                //プレイヤー3 は最初は、生存
  mainrenderer = SDL_CreateRenderer(mainwindow, -1, 0); //メインウィンドウに対するレンダラー生成
  MakeMap();
}

void Input()
{
  switch (inputevent.type)
  {
  // ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
  case SDL_JOYAXISMOTION:
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
      }
      else if (inputevent.jaxis.value < 0)
      { //上キーが押されたら
        //key.up = 1;
        //key.down = 0;
        //スティック操作(上),コマンド送信される
        joystick_send(5);
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
    else if (inputevent.jaxis.axis == 3)
    {
      //	printf("--- Four-Direction Key: Vertical Axis\n");
    }
  // ジョイスティックのボタンが押された時
  case SDL_JOYBUTTONDOWN:
    //	printf("The ID of the pressed button is %d.\n", inputevent.jbutton.button); // 押されたボタンのIDを表示（0から）
    // ボタンIDに応じた処理
    if (inputevent.jbutton.button == 11)
    {
      run = false;
    }
    if (inputevent.jbutton.button == 5)
    {
    }

    //金塊を取る
    if (inputevent.jbutton.button == 3)
    {
      //if (player[0].dst_rect.x >= 1000 && player[0].dst_rect.x <= 1100)
      if (player[myid].dst_rect.x >= 300 && player[myid].dst_rect.x <= 400)
      {
        if (player[myid].dst_rect.y >= 100 && player[myid].dst_rect.y <= 200)
        {
          //kinkai_flag = false;
          //スティック操作がされた時、金塊情報などのデータ送信される
          joystick_send(1);
        }
      }
    }

    //終了ボタンが押された
    if (inputevent.jbutton.button == 13)
    {
      run = false;
    }
    break;

  // ボタンが離された時
  case SDL_JOYBUTTONUP:
    //	printf("The ID of the released button is %d.\n",inputevent.jbutton.button); // 離されたボタンのIDを表示（0から）
    // ボタンIDに応じた処理
    if (inputevent.jbutton.button == 0)
    {
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
  for (int i = 0; i < CAMERA_NUM; i++)
  { //カメラの数だけ繰り返す
    if (camera[i].theta[0] < 90 || camera[i].theta[1] < 90)
    {
      camera[i].clockwise = false; //反時計回り
    }
    else if (camera[i].theta[0] > 270 || camera[i].theta[1] > 270)
    {
      camera[i].clockwise = true; //時計回り
    }
    if (camera[i].clockwise) {
      camera[i].theta[0]--;
      camera[i].theta[1]--;
    }
    else
    {
      camera[i].theta[0]++;
      camera[i].theta[1]++;
    }
    camera[i].tri[0][1] = camera[i].tri[0][0] + sin(camera[i].theta[0] * M_PI / 180.0) * 130; //x2の計算
    camera[i].tri[1][1] = camera[i].tri[1][0] + cos(camera[i].theta[0] * M_PI / 180.0) * 130; //y2の計算
    camera[i].tri[0][2] = camera[i].tri[0][0] + sin(camera[i].theta[1] * M_PI / 180.0) * 130; //x3の計算
    camera[i].tri[1][2] = camera[i].tri[1][0] + cos(camera[i].theta[1] * M_PI / 180.0) * 130; //x3の計算
  }
}

void RenderWindow(void) //画面の描画(イベントが無い時)
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer);                            // 設定した描画色(白)でレンダラーをクリア
  for (int i = 0; i < KOTEI_OBJECT_NUM; i++)
  {
    //描画対象が金塊で、金塊が地面に設置されていなければ、描画しない
    if (!(kotei_objects[i].type == TYPE_KINKAI))
    {
      SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); //固定オブジェクトをレンダーに出力(毎回描画しないといけない？)
    }
  }
  for (int i = 0; i < PLAYER_NUM; i++)
  {
    //描画対象がプレイヤーで、プレイヤーが消えていれば(カメラにバレるなどして)、描画しない
    if (player_flag[i] == true)
    {
      SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); //プレイヤーをレンダーに出力
    }
  }
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    SDL_RenderCopy(mainrenderer, enemy[i].image_texture, &enemy[i].src_rect, &enemy[i].dst_rect); //敵をレンダーに出力
  }
  //filledCircleColor(mainrenderer, circle_x, circle_y, 9, 0xff0000ff); //丸の描画
  for (int i = 0; i < CAMERA_NUM; i++)
  {
    filledTrigonColor(mainrenderer, camera[i].tri[0][0], camera[i].tri[1][0], camera[i].tri[0][1], camera[i].tri[1][1], camera[i].tri[0][2], camera[i].tri[1][2], 0xff0000ff);
  }
  // filledTrigonColor(mainrenderer,640,480,700,400,580,400,0xff0000ff);
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
        if (judge)
        {
          //run = false;
          player_flag[myid] = false;
          joystick_send(2); //プレイヤーが消えたことが他のクライアントに通知される。
          camera[i].tri[0][j] = camera_before[i].tri[0][j];
          camera[i].tri[1][j] = camera_before[i].tri[1][j];
          camera[i].tri[0][k] = camera_before[i].tri[0][k];
          camera[i].tri[1][k] = camera_before[i].tri[1][k];
          break;
        }
      }
    }
  }
  //ここまでカメラの判定
}

void MoveChara()
{
  int move_distance = PLAYER_SPEED * 2;
  for (int i = 0; i < 3; i++)
  {
    if (player[i].key.left)
    {
      //player[myid].dst_rect.x -= move_distance;
      //if(player[myid].dst_rect.x <0) player[myid].dst_rect.x = 0;
      player[i].dst_rect.x -= move_distance;
      if (player[i].dst_rect.x < 0)
        player[i].dst_rect.x = 0;
      //joystick_send(0); //座標などのデータ送信される
    }
    else if (player[i].key.right)
    {
      //player[myid].dst_rect.x += move_distance;
      //if(player[myid].dst_rect.x > WINDOWWIDTH - player[0].dst_rect.w) player[myid].dst_rect.x = WINDOWWIDTH - player[0].dst_rect.w;
      player[i].dst_rect.x += move_distance;
      if (player[i].dst_rect.x > WINDOWWIDTH - player[0].dst_rect.w)
        player[i].dst_rect.x = WINDOWWIDTH - player[0].dst_rect.w;
      //joystick_send(0); //座標などのデータ送信される
    }
    //else if (player[i].key.up)
    if (player[i].key.up)
    {
      //player[myid].dst_rect.y -= move_distance;
      //if(player[myid].dst_rect.y < 0) player[myid].dst_rect.y = 0;
      player[i].dst_rect.y -= move_distance;
      if (player[i].dst_rect.y < 0)
        player[i].dst_rect.y = 0;
      //joystick_send(0); //座標などのデータ送信される
    }
    else if (player[i].key.down)
    {
      player[i].dst_rect.y += move_distance;
      if (player[i].dst_rect.y > WINDOWHEIGHT - player[0].dst_rect.h)
        player[i].dst_rect.y = WINDOWHEIGHT - player[0].dst_rect.h;
      //joystick_send(0); //座標などのデータ送信される
    }

    //棚との衝突判定
    for (int i = 0; i < KOTEI_OBJECT_NUM; i++)
    {
      for (int j = 0; j < PLAYER_NUM; j++)
      {
        if (SDL_HasIntersection(&(kotei_objects[i].dst_rect), &(player[j].dst_rect))) // プレイヤーと固定オブジェクトが重なった時
        {
          if (kotei_objects[i].type != TYPE_SHELF) // 棚以外とぶつかったときは無視
            break;
          // ぶつかったぶんの距離プレイヤーの位置を戻す
          if (player[j].key.left)
          {
            player[j].dst_rect.x += move_distance;
          }
          if (player[j].key.right)
          {
            player[j].dst_rect.x -= move_distance;
          }
          if (player[j].key.up)
          {
            player[j].dst_rect.y += move_distance;
          }
          if (player[j].key.down)
          {
            player[j].dst_rect.y -= move_distance;
          }
        }
      }
    }
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
      fprintf(stderr,"map0[%d][%d]  = %d\n",j,i,loadmap_objecttype);
      if (loadmap_objecttype == TYPE_ENEMY) // 読み込んだマップデータが敵のとき
      {
        //構造体enemyに、敵の情報を格納
        enemy_index = InitObjectFromMap(enemy_index, loadmap_objecttype,dst);
      }
      else if(loadmap_objecttype == TYPE_PLAYER1 || loadmap_objecttype == TYPE_PLAYER2 || loadmap_objecttype == TYPE_PLAYER3){ // 読み込んだマップデータがプレイヤーのとき
        //構造体playerに、プレイヤーの情報を格納
        player_index = InitObjectFromMap(player_index,loadmap_objecttype,dst);
      }
      else if(loadmap_objecttype == TYPE_KINKAI || loadmap_objecttype == TYPE_SHELF)
      {
        // 棚、出入り口、金塊の情報をkotei_objectに格納
        index = InitObjectFromMap(index, loadmap_objecttype,dst);
      }
    }
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
    printf("myid = %d\n", myid);
    printf("Player 0 : axis x = %d, axis y = %d\n", player[0].dst_rect.x, player[0].dst_rect.y);
    printf("Player 1 : axis x = %d, axis y = %d\n", player[1].dst_rect.x, player[1].dst_rect.y);
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
  send_data(&data, sizeof(CONTAINER)); //クライアントのデータを送信
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
    if (myid != data.cid)
    {
      player[data.cid].dst_rect.x = data.zahyo_x; //クライアントのx座標を各プレイヤーの座標を反映
      player[data.cid].dst_rect.y = data.zahyo_y; //クライアントのy座標を各プレイヤーの座標を反映
    }
    //fprintf(stderr, "client[%d], name : %s,zahyo_x = %d, zahyo_y = %d \n", data.cid, clients[data.cid].name, data.zahyo_x, data.zahyo_y);
    result = 1;
    break;
  case KINKAI_COMMAND: //'K'のとき
    fprintf(stderr, "client[%d], name : %s, get kinkai !!!!! \n", data.cid, clients[data.cid].name);
    //kinkai_flag = false;
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
  case QUIT_COMMAND: //'Q'のとき
    fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
    result = 0;
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
  else if(loadmap_objecttype == TYPE_PLAYER1 || loadmap_objecttype == TYPE_PLAYER2 || loadmap_objecttype == TYPE_PLAYER3){
    //構造体playerに、敵の情報を格納
    player[index].type = loadmap_objecttype;
    s = IMG_Load(imgfiles[loadmap_objecttype]);
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
