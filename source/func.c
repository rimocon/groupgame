#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "../common.h"
#include "../constants.h"

inputkeys key; //inputkeys構造体をinputという名前で実体化

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

//画像ファイルパス
static char *imgfiles[TYPE_NUM] = {"./images/kinkai.png", "./images/shelf.png", "./images/camera.png", "./images/entrance.png", "./images/enemy.png", "./images/player.png", "./images/player2.png"}; // 読み込む画像ファイルを指定

// 金塊、カメラ、棚、出入り口の動かない画面に固定のオブジェクトたちの情報を格納した「kotei_objects」という実体を作る
// 金塊、カメラ、棚、出入り口の数を設定する(あとからテキストファイルにしたりしてステージごとに作ったりできる？)
//const int KOTEI_OBJECT_NUM =  KINKAI_NUM + CAMERA_NUM + SHELF_NUM + ENTRANCE_NUM;
static objectinfo kotei_objects[KOTEI_OBJECT_NUM];

// 固定オブジェクト、プレイヤーの初期位置を設定する
static SDL_Rect kinkai_dst_rects[KINKAI_NUM] = {
    //{1000, 100, 100, 100}
    {300, 100, 100, 100}};
static SDL_Rect camera_dst_rects[CAMERA_NUM] = {
    {1200, 900, 80, 60}};
static SDL_Rect shelf_dst_rects[SHELF_NUM] = {
    {400, 100, 46, 108}};
static SDL_Rect entrance_dst_rects[ENTRANCE_NUM] = {
    {0, WINDOWHEIGHT - 20, 80, 20}};
static SDL_Rect player_dst_rects[PLAYER_NUM] = {
    //{150, 850, 24, 24},
    //{150, 1050, 24, 24}
    {150, 350, 24, 24},
    {250, 350, 24, 24}};
static SDL_Rect enemy_dst_rects[ENEMY_NUM] = {
    {200, 850, 24, 24},
    {400, 850, 24, 24},
};
static int enemy_destination[ENEMY_NUM][2] = {
  {200,450},
  {400,300}
};
static int enemy_lookangles[ENEMY_NUM]={
  90,270
};


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

  /*
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    SDL_Quit();
  //Imageload(); // 画像の読み込みを行う
  */
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
  run = true;                                           //動かす
  kinkai_flag = true;                                   //金塊は最初は、配置されている
  mainrenderer = SDL_CreateRenderer(mainwindow, -1, 0); //メインウィンドウに対するレンダラー生成

  Imageload();
}

void Input()
{
  switch (inputevent.type)
  {

  // ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
  case SDL_JOYAXISMOTION:
    //joystick_send(0); //座標などのデータ送信される
    //printf("The axis ID of the operated key is %d.\n",inputevent.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
    if (inputevent.jaxis.axis == 0)
    {
      //printf("--- Analog-Direction Key: 0 Axis\n");
      if (inputevent.jaxis.value > 0)
      { //右キーが押されたら
        key.right = 1;
        key.left = 0;
        key.up = 0;
        key.down = 0;
      }
      else if (inputevent.jaxis.value < 0)
      { //左キーが押されたら
        //printf("press left\n");
        key.right = 0;
        key.left = 1;
        key.up = 0;
        key.down = 0;
      }
      else if (inputevent.jaxis.value == 0)
      { //真ん中にスティックが戻ったら
        //printf("reverse center\n");
        key.right = 0;
        key.left = 0;
        key.up = 0;
        key.down = 0;
      }
    }
    else if (inputevent.jaxis.axis == 1)
    {
      //printf("--- Analag-Direction Key: 1 Axis\n");
      if (inputevent.jaxis.value > 0)
      { //下キーが押されたら
        //printf("press down\n"); //確認用
        key.right = 0;
        key.left = 0;
        key.up = 0;
        key.down = 1;
      }
      else if (inputevent.jaxis.value < 0)
      { //上キーが押されたら
        //printf("press up\n"); //確認用
        key.right = 0;
        key.left = 0;
        key.up = 1;
        key.down = 0;
      }
      else if (inputevent.jaxis.value == 0)
      { //真ん中にスティックが戻ったら
        //printf("reverse center\n");
        key.right = 0;
        key.left = 0;
        key.up = 0;
        key.down = 0;
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

    //スティック操作がされた時、座標などのデータ送信される
    //joystick_send(0);

    break;
  // ジョイスティックのボタンが押された時
  case SDL_JOYBUTTONDOWN:
    //	printf("The ID of the pressed button is %d.\n", inputevent.jbutton.button); // 押されたボタンのIDを表示（0から）
    // ボタンIDに応じた処理
    if (inputevent.jbutton.button == 0)
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
      if (player[0].dst_rect.x >= 300 && player[0].dst_rect.x <= 400)
      {
        if (player[0].dst_rect.y >= 100 && player[0].dst_rect.y <= 200)
        {
          kinkai_flag = false;
          //スティック操作がされた時、金塊情報などのデータ送信される
          joystick_send(1);
        }
      }
      /*
      if (player[1].dst_rect.x >= 1000 && player[1].dst_rect.x <= 1100)
      {
        if (player[1].dst_rect.y >= 100 && player[1].dst_rect.y <= 200)
        {
          kinkai_flag = false;
          //スティック操作がされた時、金塊情報などのデータ送信される
          joystick_send(1);
        }
      }
      */
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
    /*
  case SDL_KEYDOWN: // キーボードのキーが押された時
  
    // 押されたキーの名前等を表示
    printf("The pressed key is %s.\n", SDL_GetKeyName(inputevent.key.keysym.sym));
    printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), inputevent.key.keysym.mod);
    // 押されたキーごとに処理
    switch (inputevent.key.keysym.sym)
    {
    
    case SDLK_UP:
      printf("press up\n");
      key.right = 0;
      key.left = 0;
      key.up = 1;
      key.down = 0;
      //circle_y = circle_y - 10; //プレイヤーの座標を上方向に変更
      break;
    case SDLK_DOWN:
      printf("press down\n");
      key.right = 0;
      key.left = 0;
      key.up = 0;
      key.down = 1;
      //circle_y = circle_y + 10; //プレイヤーの座標を下方向に変更
      break;
    case SDLK_RIGHT:
      printf("press right\n");
      key.right = 1;
      key.left = 0;
      key.up = 0;
      key.down = 0;
      //circle_x = circle_x + 10; //プレイヤーの座標を右方向に変更
      break;
    case SDLK_LEFT:
      printf("press left\n");
      key.right = 0;
      key.left = 1;
      key.up = 0;
      key.down = 0;
      //circle_x = circle_x - 10; //プレイヤーの座標を左方向に変更
      break;
    case SDLK_SPACE: //金塊を取る
      if (player[0].dst_rect.x >= 1000 && player[0].dst_rect.x <= 1100)
      {
        if (player[0].dst_rect.y >= 100 && player[0].dst_rect.y <= 200)
        {
          kinkai_flag = false;
        }
      }
      break;
      
    case SDLK_1:
      run = false;
      break;
    }
    break;
    */
  }
  //printf("%d, %d\n", player[0].dst_rect.x, player[0].dst_rect.y); //プレイヤーの座標確認用
}

void Destroy()
{
  SDL_DestroyWindow(mainwindow);
  SDL_DestroyRenderer(mainrenderer);
}

void Imageload()
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer);                            // 設定した描画色(白)でレンダラーをクリア

  SDL_Surface *s; // サーフェイスを一時的に保存する変数

  // 順番的に先に背景を描画する必要あり

  // 「kotei_objects」構造体（金塊、カメラ、棚、出入り口）にグローバル変数で設定した位置、画像をに設定する
  int i = 0;
  for (int j = 0; j < KINKAI_NUM; j++)
  {
    printf("j = %d\n", j);
    kotei_objects[i].type = TYPE_KINKAI;
    s = IMG_Load(imgfiles[TYPE_KINKAI]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    kotei_objects[i].dst_rect = kinkai_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    i++;                                                                                                                  // 金塊、カメラ、棚、出入り口のデータをkotei_objectsに保存するためにiをインクリメント
  }

  for (int j = 0; j < SHELF_NUM; j++)
  {
    printf("j = %d\n", j);
    kotei_objects[i].type = TYPE_SHELF;
    s = IMG_Load(imgfiles[TYPE_SHELF]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    kotei_objects[i].dst_rect = shelf_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    i++;
  }

  for (int j = 0; j < ENTRANCE_NUM; j++)
  {
    printf("j = %d\n", j);
    kotei_objects[i].type = TYPE_ENTRANCE;
    s = IMG_Load(imgfiles[TYPE_ENTRANCE]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    printf("j = %d\n", j);
    kotei_objects[i].dst_rect = entrance_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    i++;
  }
  for (int j = 0; j < CAMERA_NUM; j++)
  {
    printf("j = %d\n", j);
    kotei_objects[i].type = TYPE_CAMERA;
    s = IMG_Load(imgfiles[TYPE_CAMERA]);
    kotei_objects[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    kotei_objects[i].src_rect.x = 0;
    kotei_objects[i].src_rect.y = 0;
    kotei_objects[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    kotei_objects[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    kotei_objects[i].dst_rect = camera_dst_rects[j];
    SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
  }
  for (i = 0; i < PLAYER_NUM; i++)
  {
    player[i].type = TYPE_PLAYER;
    s = IMG_Load(imgfiles[TYPE_PLAYER + i]);
    player[i].image_texture = SDL_CreateTextureFromSurface(mainrenderer, s);
    player[i].src_rect.x = 0;
    player[i].src_rect.y = 0;
    player[i].src_rect.w = s->w; // 読み込んだ画像ファイルの幅を元画像の領域として設定
    player[i].src_rect.h = s->h; // 読み込んだ画像ファイルの高さを元画像の領域として設定
    player[i].dst_rect = player_dst_rects[i];
    SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); // ヘッダファイルで指定した領域で、テクスチャからレンダラーに出力
    player[i].speed = PLAYER_SPEED;
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
}

void RenderWindow(void) //画面の描画(イベントが無い時)
{
  SDL_SetRenderDrawColor(mainrenderer, 255, 255, 255, 255); // 生成したレンダラーに描画色として白を設定
  SDL_RenderClear(mainrenderer);                            // 設定した描画色(白)でレンダラーをクリア
  for (int i = 0; i < KOTEI_OBJECT_NUM; i++)
  {
    //描画対象が金塊で、金塊が地面に設置されていなければ、描画しない
    if (!(kotei_objects[i].type == TYPE_KINKAI && kinkai_flag == false))
    {
      SDL_RenderCopy(mainrenderer, kotei_objects[i].image_texture, &kotei_objects[i].src_rect, &kotei_objects[i].dst_rect); //固定オブジェクトをレンダーに出力(毎回描画しないといけない？)
    }
  }
  for (int i = 0; i < PLAYER_NUM; i++)
  {
    SDL_RenderCopy(mainrenderer, player[i].image_texture, &player[i].src_rect, &player[i].dst_rect); //プレイヤーをレンダーに出力
  }
  for (int i = 0; i < ENEMY_NUM; i++)
  {
    SDL_RenderCopy(mainrenderer, enemy[i].image_texture, &enemy[i].src_rect, &enemy[i].dst_rect); //敵をレンダーに出力
  }
  SDL_RenderPresent(mainrenderer); // 描画データを表示
}

void MoveChara()
{
  int move_distance = PLAYER_SPEED * 2;
  if (key.left)
  {
    player[myid].dst_rect.x -= move_distance;
    joystick_send(0); //座標などのデータ送信される
  }
  else if (key.right)
  {
    player[myid].dst_rect.x += move_distance;
    joystick_send(0); //座標などのデータ送信される
  }
  else if (key.up)
  {
    player[myid].dst_rect.y -= move_distance;
    joystick_send(0); //座標などのデータ送信される
  }
  else if (key.down)
  {
    player[myid].dst_rect.y += move_distance;
    joystick_send(0); //座標などのデータ送信される
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
      if(enemy[i].dst_rect.x + ENEMY_SPEED > 1256){
        enemy[i].look_angle = 270;
        break;
      }
      enemy[i].dst_rect.x += ENEMY_SPEED;
      break;
    case 180:
      enemy[i].dst_rect.y += ENEMY_SPEED;
      break;
    case 270:
      if(enemy[i].dst_rect.x - ENEMY_SPEED < 0){
        enemy[i].look_angle = 90;
        break;
      }
      enemy[i].dst_rect.x -= ENEMY_SPEED;
      break;
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

void joystick_send(int num)
{
  CONTAINER data;
  memset(&data, 0, sizeof(CONTAINER)); //dataの初期化
  if (num == 0)
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
  else if (num == 1)
  {
    //コマンドとして、座標の'K'を代入
    data.command = KINKAI_COMMAND; //コマンドを格納
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
    //send_data(BROADCAST, &data, sizeof(data));
    kinkai_flag = false;
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
    fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);
    exit(1); //異常終了
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