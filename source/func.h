/*  二重インクルード防止  */
#ifndef _FUNC_H_
#define _FUNC_H_

/*  変数  */
TTF_Font *japanesefont; //フォント用
int position;
bool up, down;
/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define PLAYER_NUM 3 // オブジェクトの数などはテキストファイルで読み込めるようにしたほうがいろんなマップに対応できるから後から修正したい
#define PLAYER_SPEED 4

// 金塊、カメラ、棚、出入り口の数
#define CAMERA_NUM 5
//#define CAMERA_NUM 0
#define BACKGROUND_NUM 1
#define FONT_NUM 6

#define ENEMY_SPEED 1

#define KOTEI_OBJECT_NUM_MAX 300 // 固定オブジェクトの最大の数

#define MAP_CHIPSIZE 64 //変数map0の、1マス分のピクセルの大きさ（仮置き）
#define MAP_WIDTH 20    // 変数map0の横の数、ゲーム画面を横に20等分してる
#define MAP_HEIGHT 15   // 変数map0の縦の数、ゲーム画面を縦に16等分してる

//催涙スプレーの幅と高さと敵が固まる時間を設定する
#define SPRAY_WIDTH 80
#define SPRAY_HEIGHT 50
#define SAIRUI_TIME 3000
#define SPRAY_TIME 300 // スプレーが使える時間

#define HACKTIME 2000 //ハッキングに要する時間
#define STOPTIME 2000 //ハッキング中にカメラを止める時間
/*  変数  */
bool kinkai_flag;      //金塊を描画するかしないか
bool hacking_flag;     //金塊を描画するかしないか
bool kinkai_keep_flag; //プレイヤーが金塊を持っているかどうか
bool player_flag[3];   //プレイヤーを描画するか、しないか

int before_enemy_x[ENEMY_NUM];  //敵の前の座標 x
int before_enemy_y[ENEMY_NUM];  //敵の前の座標 y
Uint32 random_start[ENEMY_NUM]; //ランダム移動を開始した時間を格納

int time_now, time_left;
float gauge;
/*  mapデータ */
// static int map0[MAP_HEIGHT][MAP_WIDTH] = {
// 	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
// 	{2, 0, 0, 0, 0, 0, 0, 0, 12,0, 0, 0, 11,0, 0, 0, 0, 0, 0, 2},
// 	{2, 0, 13,0, 5, 0, 0, 12,0, 0, 0, 0, 0, 11,0, 0, 0, 13,0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
// 	{2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
// 	{2, 0, 10,0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 10,0, 0, 0, 9, 0, 2},
// 	{2, 6, 7, 8, 0, 0, 0, 0, 10,0, 5, 0, 9, 0, 0, 0, 0, 0, 0, 2},
// 	{2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
// };

/*  mapデータ */

//ステージ1用
/*
static int map0[MAP_HEIGHT][MAP_WIDTH] = {
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 2},
	{2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 2},
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
	{2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
	{2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
	{2, 5, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
	{2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
	{2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
	{2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 0, 0, 2},
	{2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
	{2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
	{2, 0, 8, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 5, 0, 0, 0, 0, 0, 2},
	{2, 6, 7, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 2},
	{2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};
*/
//ステージ1用
static int map0[MAP_HEIGHT][MAP_WIDTH] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {2, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 2},
    {2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 6, 7, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};

//ステージ2用
static int map1[MAP_HEIGHT][MAP_WIDTH] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 2},
    {2, 2, 2, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 0, 0, 2},
    {2, 1, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 8, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 5, 0, 0, 0, 0, 0, 2},
    {2, 6, 7, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};

//ステージ3用
static int map2[MAP_HEIGHT][MAP_WIDTH] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 2},
    {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 5, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2},
    {2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 8, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 5, 0, 0, 0, 0, 0, 2},
    {2, 6, 7, 2, 2, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 2},
    {2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};

typedef enum {
  TYPE_NONE = 0,
  TYPE_KINKAI = 1,
  TYPE_SHELF = 2,
  TYPE_CAMERA = 3,
  TYPE_ENTRANCE = 4,
  TYPE_ENEMY = 5,
  TYPE_PLAYER1 = 6, // TYPEを追加する場合はPLAYERとMOVING_FLOORの間にする
  TYPE_PLAYER2 = 7,
  TYPE_PLAYER3 = 8,

  TYPE_ENEMY_MOVING_FLOOR_UL = 9,   // UL = UpLeftの略 TYPE_ENEMY_MOVING_FLOOR_UR >= iって条件にしてるため、これ以降は移動床の宣言
  TYPE_ENEMY_MOVING_FLOOR_UR = 10,  // UR = UpRightの略
  TYPE_ENEMY_MOVING_FLOOR_DL = 11,  // DL = DownLeftの略
  TYPE_ENEMY_MOVING_FLOOR_DR = 12,  // DR = DownRightの略
  TYPE_ENEMY_MOVING_FLOOR_REV = 13, // REV = Reverseの略です
  TYPE_BACKGROUND = 14,
  TYPE_SPRAY = 15,
  TYPE_NUM = 16
} objecttype;

typedef struct
{
  objecttype type;
  SDL_Texture *image_texture;
  SDL_Rect src_rect;
  SDL_Rect dst_rect;
} objectinfo;

typedef struct
{              //キー入力用の構造体を型宣言
  Uint32 left, //左矢印
      right,   //右矢印
      up,      //上矢印
      down,    //下矢印
      a,       //4ボタン(決定ボタン)
      x,       //2ボタン(催涙スプレー)
      y;       //1ボタン(ハッキングボタン)
} inputkeys;

typedef struct
{
  objecttype type;
  SDL_Texture *image_texture;
  SDL_Texture *spray_texture;
  SDL_Rect src_rect;
  SDL_Rect dst_rect;
  float back_zahyo_x;
  float back_zahyo_y;
  bool flag_kinkai;
  bool flag_hack_start;
  bool flag_hack_end;
  int hack; //ハッキングできる回数
  int inputtime;
  int speed;
  inputkeys key; //inputkeys構造体をinputという名前で実体化
  int look_angle;
  int spray_flag;
  SDL_Rect spray_src_rect;
  SDL_Rect spray_dst_rect;
  int spray_hitlines[2][4];
  SDL_Point spray_origin;
  int spraytime;
} playerinfo;

typedef struct
{
  SDL_Texture *image_texture;
  SDL_Rect src_rect;
  SDL_Rect dst_rect;
  bool flag_kinkai;
  bool flag_hack;
  int tri[2][3];
  float theta[3];
  bool clockwise;
  double angle;
} camerainfo;

typedef enum {
  MT_NONE = 0,
  MT_MOVING_FLOOR = 1,        // 移動床に沿って移動する
  MT_RIGHT_METHOD = 2,        // 右手法で動く
  MT_RANDOM = 3,              // MAP_CHIPSIZEごとにランダムな方向へ動く
  MT_TRACKING = 4,            // プレイヤーを追跡するように動く
  MT_RANDOM_AND_TRACKING = 5, //プレイヤーがNPCに一定距離近づいた場合、追跡する
  MT_STOP = 6                 //ずっと動かない
} enemymovetype;              // 敵の動きのタイプをこの中から選ぶ

typedef struct
{
  objecttype type;            // タイプ
  SDL_Texture *image_texture; // テクスチャ
  SDL_Rect src_rect;          // 元画像の座標、範囲
  SDL_Rect dst_rect;          // 出力先の座標、範囲
  SDL_Rect prev_overlap_rect; // 移動床内で無限ループしないようにするため、一時的に重なった範囲を保存する変数
  bool flag_kinkai;           // 金塊をとったかどうか
  int speed;                  //敵の移動速度
  int move_angle;             // 敵が動く方向
  bool flag_sairui;           // 止まってるかどうか
  enemymovetype movetype;     // 敵の動きのタイプ
  unsigned int savetime;      // 時間を保存する
  int tri[2][3];              // 視界の三角形
  int prev_angle;
} enemyinfo; // 敵の構造体

typedef struct
{
  SDL_Texture *image_texture;
  SDL_Rect src_rect;
  SDL_Rect dst_rect;
} backgroundinfo;

typedef struct
{
  SDL_Texture *image_texture;
  SDL_Rect src_rect;
  SDL_Rect dst_rect;
} fontinfo;

/* グローバル変数 */
//ネット関連
static int num_clients;
//static int myid;
static int sock;
static int num_sock;
static fd_set mask; //FD集合を表す構造体

//画像ファイルパス
//static char *imgfiles[TYPE_NUM] = {"","./images/kinkai.png","./images/shelf.png","./images/camera.png","./images/entrance.png","./images/enemy.png","./images/player.png", "./images/player2.png", "./images/player3.png","./images/floor_ul.png","./images/floor_ur.png","./images/floor_dl.png","./images/floor_dr.png","./images/floor_rev.png","./images/menu.png","./images/spray.png"}; // 読み込む画像ファイルを指定
static char *imgfiles[TYPE_NUM] = {"", "./images/kinkai.png", "./images/shelf.png", "./images/camera.png", "./images/entrance.png", "./images/enemy.png", "./images/player1_8pattern.png", "./images/player2_8pattern.png", "./images/player3_8pattern.png", "./images/floor_ul.png", "./images/floor_ur.png", "./images/floor_dl.png", "./images/floor_dr.png", "./images/floor_rev.png", "./images/menu.png", "./images/spray.png"}; // 読み込む画像ファイルを指定

//フォント
static char *fonts[FONT_NUM] = {"開始", "終了", "STAGE 1", "STAGE 2", "STAGE 3", "4番ボタンで開始！"};

static SDL_Rect camera_dst_rects[CAMERA_NUM] = {
    {700, 200, 120, 100},
    {600, 400, 120, 100},
    {800, 100, 120, 100},
    {500, 800, 120, 100},
    {600, 300, 120, 100}};

static SDL_Rect camera_dst_rects_Stage2[CAMERA_NUM] = {
    {200, 200, 120, 100},
    {1000, 200, 120, 100},
    {200, 800, 120, 100},
    {1000, 200, 120, 100},
    {600, 300, 120, 100}};

static SDL_Rect camera_dst_rects_Stage3[CAMERA_NUM] = {
    {400, 400, 120, 100},
    {1000, 400, 120, 100},
    {400, 600, 120, 100},
    {1000, 600, 120, 100},
    {600, 300, 120, 100}};

// 敵が最初に向いている方向,敵の動きのタイプを指定する
static int enemy_moveangles[ENEMY_NUM] = {
    180,
    90};

static enemymovetype enemy_movetypes[ENEMY_NUM] = { //NPCの動きのパターンを選択
    MT_RANDOM_AND_TRACKING,
    MT_RANDOM_AND_TRACKING,
    MT_RANDOM_AND_TRACKING,
    MT_RANDOM_AND_TRACKING,
    MT_RANDOM_AND_TRACKING};
static SDL_Rect font_dst_rects[FONT_NUM] = {
    {540, 380, 0, 0},
    {540, 530, 0, 0},
    {540, 380, 0, 0},
    {540, 380, 0, 0},
    {540, 380, 0, 0},
    {400, 530, 0, 0}
    };

static CLIENT clients[MAX_NUM_CLIENTS];

playerinfo player[PLAYER_NUM]; // プレイヤーの情報を格納したplayer構造体を実体化
camerainfo camera[CAMERA_NUM];
/*カメラの当たり判定に使っているライブラリが、何かに接触すると、その物体に張り付く動作をするライブラリであったので、
  カメラの張り付く前の座標を保持しておき、張り付いた後に、カメラの座標に代入することにより、カメラを固定する。*/
camerainfo camera_before[CAMERA_NUM];
enemyinfo enemy[ENEMY_NUM];
backgroundinfo background[BACKGROUND_NUM];             //背景の情報を実体化
fontinfo font[FONT_NUM];                               //フォントの情報を実体化
static objectinfo kotei_objects[KOTEI_OBJECT_NUM_MAX]; // 金塊、カメラ、棚、出入り口の動かない画面に固定のオブジェクトたちの情報を格納した「kotei_objects」という実体を作る
int kotei_object_num;
int savestopenemy[ENEMY_NUM];
int lrflag = 0;

/*  関数のプロトタイプ宣言 */
extern void SetCamera(void);                                                          //カメラの値など初期化
extern void Imageload(void);                                                          //画像読み込み関数
extern float Rotation(int x1, int y1, int a, int b, double theta, int *x2, int *y2);  //回転計算
extern int InitObjectFromMap(int index, objecttype loadmap_objecttype, SDL_Rect dst); // マップデータを読み込んでその位置にオブジェクトを初期化する関数
extern int ChangeEnemyMoveAngle(enemyinfo *e, SDL_Rect movefloor, objecttype type);
extern void Fontload(void); //フォント読み込み

static int input_command(void);
static int execute_command(void);
static void send_data(void *, int);
static int receive_data(void *, int);
static void handle_error(char *);
#endif
