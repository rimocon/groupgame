/*  二重インクルード防止  */
#ifndef _FUNC_H_
#define _FUNC_H_

/*  変数  */
TTF_Font *japanesefont; //フォント用
int position;
bool up,down;
/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define PLAYER_NUM 3 // オブジェクトの数などはテキストファイルで読み込めるようにしたほうがいろんなマップに対応できるから後から修正したい
#define PLAYER_SPEED 1

// 金塊、カメラ、棚、出入り口の数
#define KINKAI_NUM 2
#define SHELF_NUM 10 // 棚の数、マップデータ(map0)の棚の数と合わせる
#define ENTRANCE_NUM 3
#define  KOTEI_OBJECT_NUM 16 // KINKAI_NUM + CAMERA_NUM + SHELF_NUM + ENTRANCE_NUMを足したもの

#define CAMERA_NUM 5
#define BACKGROUND_NUM 1
#define FONT_NUM 2
#define ENEMY_NUM 2
#define ENEMY_SPEED 1

// 各オブジェクトの最大数、実装予定
// #define SHELF_NUM_MAX 100 //
// #define  KOTEI_OBJECT_NUM_MAX 125 // KINKAI_NUM + CAMERA_NUM + SHELF_NUM + ENTRANCE_NUMを足したもの

//敵の最大数と敵のスピード、実装予定
// #define ENEMY_NUM_MAX 2
// #define ENEMY_SPEED 1

#define MAP_CHIPSIZE 64 //変数map0の、1マス分のピクセルの大きさ（仮置き）
#define MAP_WIDTH 20 // 変数map0の横の数、ゲーム画面を横に20等分してる
#define MAP_HEIGHT 15 // 変数map0の縦の数、ゲーム画面を縦に16等分してる

/*  変数  */
bool kinkai_flag; //金塊を描画するかしないか
bool player_flag[3]; //プレイヤーを描画するか、しないか
/*  mapデータ */
static int map0[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/*  構造体宣言  */
// typedef enum{
// 	TYPE_KINKAI = 0,
// 	TYPE_SHELF = 1,
// 	TYPE_CAMERA = 2,
// 	TYPE_ENTRANCE = 3,
// 	TYPE_ENEMY = 4,
// 	TYPE_PLAYER = 5,
// 	TYPE_NUM = 6
// }objecttype;
typedef enum{
	TYPE_NONE = 0,
	TYPE_KINKAI = 1,
	TYPE_SHELF = 2,
	TYPE_CAMERA = 3,
	TYPE_ENTRANCE = 4,
	TYPE_ENEMY = 5,
	TYPE_PLAYER1 = 6, // TYPEを追加する場合はPLAYERとMOVING_FLOORの間にする
	TYPE_PLAYER2 = 7,
	TYPE_PLAYER3 = 8,
	TYPE_ENEMY_MOVING_FLOOR_UR = 9, // TYPE_ENEMY_MOVING_FLOOR_UR >= iって条件にしてるため、これ以降は移動床の宣言
	TYPE_ENEMY_MOVING_FLOOR_UL = 10,
	TYPE_ENEMY_MOVING_FLOOR_DL = 11,
	TYPE_ENEMY_MOVING_FLOOR_DR = 12,
	TYPE_ENEMY_MOVING_FLOOR_REV = 13,
  TYPE_BACKGROUND = 14,
	TYPE_NUM = 15
}objecttype;

typedef struct {
	objecttype type;
	SDL_Texture *image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
} objectinfo;

typedef struct { //キー入力用の構造体を型宣言
	Uint32  left, //左矢印
		right, //右矢印
		up, //上矢印
		down, //下矢印
		a;  //4ボタン(決定ボタン)
}inputkeys;

typedef struct {
	objecttype type;
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
	int speed;
  inputkeys key; //inputkeys構造体をinputという名前で実体化
}playerinfo;


typedef struct {
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
  int tri[2][3];
  float theta[2];
  bool clockwise;
  double angle;
}camerainfo;

typedef struct {
	int rotate_range; // 敵の回転速度
	objecttype type; // タイプ
	SDL_Texture * image_texture; // テクスチャ
	SDL_Rect src_rect; // 元画像の座標、範囲
	SDL_Rect dst_rect; // 出力先の座標、範囲
	SDL_Rect prev_overlap_rect; // 移動床内で無限ループしないようにするため、一時的に重なった範囲を保存する変数
	bool flag_kinkai; // 金塊をとったかどうか
	int speed; //敵の移動速度
	int look_angle; // 敵が向いている方向(0度〜360度)、視野の描画する方法によるので仮
	int move_angle; // 敵が動く方向
	bool isgodest; // 目的地まで行ってるかどうか
}enemyinfo; // 敵の構造体

typedef struct {
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
}backgroundinfo;

typedef struct {
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
}fontinfo;



/* グローバル変数 */
//ネット関連
static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask; //FD集合を表す構造体

//画像ファイルパス
static char *imgfiles[TYPE_NUM] = {"","./images/kinkai.png","./images/shelf.png","./images/camera.png","./images/entrance.png","./images/enemy.png","./images/player.png", "./images/player2.png", "./images/player3.png","empty1","empty2","empty3","emputy4","empty5","./images/menu.png"}; // 読み込む画像ファイルを指定
//フォント
static char *fonts[FONT_NUM] = {"開始","終了"};

static SDL_Rect camera_dst_rects[CAMERA_NUM] = {
  {700,200,120,100},
  {600,400,120,100},
  {800,100,120,100},
  {500,800,120,100},
  {600,300,120,100}
};


// 敵が最初に向いている方向を指定する
static int enemy_lookangles[ENEMY_NUM] = {
	90,270
};
static int enemy_moveangles[ENEMY_NUM] = {
	90,270
};
static SDL_Rect font_dst_rects[FONT_NUM] = {
  {540,380,0,0},
  {540,530,0,0}
};



static CLIENT clients[MAX_NUM_CLIENTS];

playerinfo player[PLAYER_NUM];  // プレイヤーの情報を格納したplayer構造体を実体化
camerainfo camera[CAMERA_NUM];
/*カメラの当たり判定に使っているライブラリが、何かに接触すると、その物体に張り付く動作をするライブラリであったので、
  カメラの張り付く前の座標を保持しておき、張り付いた後に、カメラの座標に代入することにより、カメラを固定する。*/
camerainfo camera_before[CAMERA_NUM];
enemyinfo enemy[ENEMY_NUM];
static objectinfo kotei_objects[KOTEI_OBJECT_NUM]; // 金塊、カメラ、棚、出入り口の動かない画面に固定のオブジェクトたちの情報を格納した「kotei_objects」という実体を作る
backgroundinfo background[BACKGROUND_NUM]; //背景の情報を実体化
fontinfo font[FONT_NUM]; //フォントの情報を実体化

/*  関数のプロトタイプ宣言 */
extern void SetCamera(void); //カメラの値など初期化
extern void Imageload(void); //画像読み込み関数
extern float Rotation(int x1, int y1,int a,int b, double theta,int *x2,int *y2); //回転計算
extern int InitObjectFromMap(int index, objecttype loadmap_objecttype, SDL_Rect dst); // マップデータを読み込んでその位置にオブジェクトを初期化する関数
extern int ChangeEnemyMoveAngle(enemyinfo *e,SDL_Rect movefloor, objecttype type);
extern void Fontload(void); //フォント読み込み

static int input_command(void);
static int execute_command(void);
static void send_data(void *, int);
static int receive_data(void *, int);
static void handle_error(char *);
#endif
