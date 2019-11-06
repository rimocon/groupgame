#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "../constants.h"
#include "../common.h"

inputkeys key; //inputkeys構造体をinputという名前で実体化

//画像ファイルパス
static char *imgfiles[TYPE_NUM] = {"./images/kinkai.png","./images/shelf.png",
                                   "./images/camera.png","./images/entrance.png",
                                   "./images/enemy.png","./images/player.png"}; // 読み込む画像ファイルを指定
static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask;
int speaker; //発言者の番号
static CLIENT clients[MAX_NUM_CLIENTS];

// 金塊、カメラ、棚、出入り口の動かない画面に固定のオブジェクトたちの情報を格納した「kotei_objects」という実体を作る
// 金塊、カメラ、棚、出入り口の数を設定する(あとからテキストファイルにしたりしてステージごとに作ったりできる？)
//const int KOTEI_OBJECT_NUM =  KINKAI_NUM + CAMERA_NUM + SHELF_NUM + ENTRANCE_NUM;
static objectinfo kotei_objects[KOTEI_OBJECT_NUM];

// 固定オブジェクト、プレイヤーの初期位置を設定する
static SDL_Rect kinkai_dst_rects[KINKAI_NUM] = {
  {1000, 100, 100, 100}
};
static SDL_Rect camera_dst_rects[CAMERA_NUM] = {
  {1200,900,80,60}
};
static SDL_Rect shelf_dst_rects[SHELF_NUM] = {
  {400, 100, 46, 108}
};
static SDL_Rect entrance_dst_rects[ENTRANCE_NUM] = {
  {0, WINDOWHEIGHT-20,80,20}
};
static SDL_Rect player_dst_rects[PLAYER_NUM] = {
  {150,850,24,24}
};

// プレイヤーの情報を格納したplayer構造体を実体化
playerinfo player[PLAYER_NUM];

void setup_client(char *, u_short);
int control_requests();
void terminate_client();

static int input_command(void);
static int execute_command(void);
static void send_data(void *, int);
static int receive_data(void *, int);
static void handle_error(char *);

void Startup() //描画に関する最初のセットアップを行う
{
  circle_x = 150, circle_y = 850; //点の初期位置

  if(SDL_Init(SDL_INIT_VIDEO) == -1){
    SDL_Quit();
  }
  Imageload(); //画像の読み込み

  mainwindow = SDL_CreateWindow(
    "test" ,
    SDL_WINDOWPOS_CENTERED ,
    SDL_WINDOWPOS_CENTERED ,
    WINDOWWIDTH,
    WINDOWHEIGHT,
    0
  );
  if(mainwindow == NULL){
    printf("ウィンドウを生成できませんでした\n");
    SDL_Quit(); //終了
  }
  run = true;

  mainrenderer = SDL_CreateRenderer(mainwindow, -1, 0);

  Imageload();
}

void setup_client(char *server_name, u_short port)
{
  struct hostent *server;
  struct sockaddr_in sv_addr;

  fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
  if ((server = gethostbyname(server_name)) == NULL)
  {
    handle_error("gethostbyname()");
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    handle_error("socket()");
  }

  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(port);
  sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0];

  if (connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
  {
    handle_error("connect()");
  }

  fprintf(stderr, "Input your name: ");
  char user_name[MAX_LEN_NAME];
  if (fgets(user_name, sizeof(user_name), stdin) == NULL)
  {
    handle_error("fgets()");
  }
  user_name[strlen(user_name) - 1] = '\0';
  send_data(user_name, MAX_LEN_NAME);

  fprintf(stderr, "Waiting for other clients...\n");
  receive_data(&num_clients, sizeof(int)); //クライアントの数
  fprintf(stderr, "Number of clients = %d.\n", num_clients);
  receive_data(&myid, sizeof(int)); //クライアントID
  fprintf(stderr, "Your ID = %d.\n", myid);
  receive_data(&speaker, sizeof(int)); //発言者の情報の読み込み

  int i;
  //クライアントのデータを読み込み
  for (i = 0; i < num_clients; i++)
  {
    receive_data(&clients[i], sizeof(CLIENT));
  }

  fprintf(stderr, "現在の発言者は，　%s　さんです.\n", clients[speaker].name); //現在の発言者を表示

  num_sock = sock + 1;
  FD_ZERO(&mask);
  FD_SET(0, &mask);
  FD_SET(sock, &mask);
}

void Input()
{
  switch (inputevent.type)
  {
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
      circle_y = circle_y - 10; //プレイヤーの座標を上方向に変更
      break;
    case SDLK_DOWN:
      printf("press down\n");
      key.right = 0;
      key.left = 0;
      key.up = 0;
      key.down = 1;
      circle_y = circle_y + 10; //プレイヤーの座標を下方向に変更
      break;
    case SDLK_RIGHT:
      printf("press right\n");
      key.right = 1;
      key.left = 0;
      key.up = 0;
      key.down = 0;
      circle_x = circle_x + 10; //プレイヤーの座標を右方向に変更
      break;
    case SDLK_LEFT:
      printf("press left\n");
      key.right = 0;
      key.left = 1;
      key.up = 0;
      key.down = 0;
      circle_x = circle_x - 10; //プレイヤーの座標を左方向に変更
      break;
    case SDLK_1:
      run = false;
    case SDLK_SPACE: //金塊を取るボタン
      if (circle_x > 1000 && circle_x < 1100 && circle_y > 100 && circle_y < 200)
      {
        flag_kinkai = false;
      }
      break;
    }
    break;
  }
  printf("%d, %d\n", circle_x, circle_y); //プレイヤーの座標確認用
}

int control_requests()
{
  fd_set read_flag = mask;

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 30;

  if (select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1)
  {
    handle_error("select()");
  }

  int result = 1;

  if (SDL_PollEvent(&inputevent))
  {                //キーボードが押された時
    printf("o\n"); //キーボードの入力に反応しているか確認用
    //cond = coord(); //座標に関する処理
    Input();
    
  }

  /*
  //クライアント自身がコマンドを送る時
  if (FD_ISSET(0, &read_flag))
  {
    //input_command　関数
    //  コマンドを受け取って、コマンドに合わせた処理をする
    //  例えば、'M'なら、メッセージが入力されるまで待つ
    result = input_command();
  }
  

  //サーバーからコマンドを受信した時
  if (FD_ISSET(sock, &read_flag))
  {
    result = execute_command();
  }
  */
  //return result;
  return 1;
}

//座標に関する関数
int coord()
{
  ZAHYO data;

  printf("%d\n", circle_x);
}

static int input_command()
{
  CONTAINER data;
  char com;
  memset(&data, 0, sizeof(CONTAINER));
  com = getchar(); //標準入力から１文字を受け取る('M'または'Q')
  while (getchar() != '\n')
    ; //改行コードが入力されるまで，1文字ずつ文字を受けとる

  switch (com)
  {
  case MESSAGE_COMMAND:
    fprintf(stderr, "Input message: ");
    if (fgets(data.message, MAX_LEN_BUFFER, stdin) == NULL)
    {
      handle_error("fgets()");
    }
    data.command = MESSAGE_COMMAND;
    data.message[strlen(data.message) - 1] = '\0';
    data.cid = myid;
    send_data(&data, sizeof(CONTAINER));
    break;
  case QUIT_COMMAND:
    data.command = QUIT_COMMAND;
    data.cid = myid;
    send_data(&data, sizeof(CONTAINER));
    break;
  default:
    fprintf(stderr, "%c is not a valid command.\n", com);
  }

  return 1;
}

static int execute_command()
{
  CONTAINER data;
  int result = 1;

  memset(&data, 0, sizeof(CONTAINER));
  receive_data(&data, sizeof(data));

  switch (data.command)
  {
  case MESSAGE_COMMAND:
    fprintf(stderr, "client[%d] %s: %s\n", data.cid, clients[data.cid].name, data.message);
    result = 1;
    fprintf(stderr, "現在の発言者は，　%s　さんです.\n", clients[data.speaker].name); //現在の発言者を表示
    break;
  case QUIT_COMMAND:
    fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
    result = 0;
    break;
  case WARNING_COMMAND: //発言権が無い人が発言した場合
    fprintf(stderr, "%s\n", data.warning_message);
    break;
  default:
    fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);
    exit(1);
  }
  return result;
}

static void send_data(void *data, int size)
{
  if ((data == NULL) || (size <= 0))
  {
    fprintf(stderr, "send_data(): data is illeagal.\n");
    exit(1);
  }

  if (write(sock, data, size) == -1)
  {
    handle_error("write()");
  }
}

static int receive_data(void *data, int size)
{
  if ((data == NULL) || (size <= 0))
  {
    fprintf(stderr, "receive_data(): data is illeagal.\n");
    exit(1);
  }

  return (read(sock, data, size));
}

static void handle_error(char *message)
{
  perror(message);
  fprintf(stderr, "%d\n", errno);
  exit(1);
}

void terminate_client()
{
  fprintf(stderr, "Connection is closed.\n");
  close(sock);
  exit(0);
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
  //filledCircleColor(mainrenderer, circle_x, circle_y, 9, 0xff0000ff); //丸の描画
  SDL_RenderPresent(mainrenderer);              // 描画データを表示
}

void MoveChara()
{
  int move_distance = PLAYER_SPEED * 2;
  if(key.left){
    player[0].dst_rect.x -= move_distance;
  }
  else if(key.right){
    player[0].dst_rect.x += move_distance;
  }
  else if(key.up){
    player[0].dst_rect.y -= move_distance;
  }
  else if(key.down){
    player[0].dst_rect.y += move_distance;
  }
}