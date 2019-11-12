/*  二重インクルード防止  */
#ifndef _FUNC_H_
#define _FUNC_H_

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define PLAYER_NUM 1
#define PLAYER_SPEED 3

// 金塊、カメラ、棚、出入り口の数
#define KINKAI_NUM 1
#define CAMERA_NUM 1
#define SHELF_NUM 10 // 棚の数、マップデータ(map0)の棚の数と合わせる
#define ENTRANCE_NUM 3
#define  KOTEI_OBJECT_NUM 16 // KINKAI_NUM + CAMERA_NUM + SHELF_NUM + ENTRANCE_NUMを足したもの

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

/*  mapデータ */
static int map0[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
	TYPE_PLAYER = 6,
	TYPE_ENEMY_MOVING_FLOOR_UR = 7,
	TYPE_ENEMY_MOVING_FLOOR_UL = 8,
	TYPE_ENEMY_MOVING_FLOOR_DL = 9,
	TYPE_ENEMY_MOVING_FLOOR_DR = 10,
	TYPE_ENEMY_MOVING_FLOOR_REV = 11,
	TYPE_NUM = 12
}objecttype;

typedef struct {
	objecttype type;
	SDL_Texture *image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
} objectinfo;

typedef struct {
	objecttype type;
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
	int speed;
}playerinfo;


typedef struct {
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
  int tri[2][3];
  float theta[2];
  bool clockwise;
}camerainfo;

typedef struct { //キー入力用の構造体を型宣言
	Uint32  left, //左矢印
	 			  right, //右矢印
				  up, //上矢印
				  down, //下矢印
          a;  //4ボタン(決定ボタン)
}inputkeys;



typedef struct {
	int rotate_range; // 敵の回転速度
	objecttype type; // タイプ
	SDL_Texture * image_texture; // テクスチャ
	SDL_Rect src_rect; // 元画像の座標、範囲
	SDL_Rect dst_rect; // 出力先の座標、範囲
	bool flag_kinkai; // 金塊をとったかどうか
	int speed; //敵の移動速度
	int look_angle; // 敵が向いている方向(0度〜360度)、視野の描画する方法によるので仮
	int move_angle;
	bool isgodest; // 目的地まで行ってるかどうか
}enemyinfo; // 敵の構造体


/* グローバル変数 */
//画像ファイルパス
static char *imgfiles[TYPE_NUM] = {"","./images/kinkai.png","./images/shelf.png","./images/camera.png","./images/entrance.png","./images/enemy.png","./images/player.png"}; // 読み込む画像ファイルを指定

// カメラの初期位置を設定する
static SDL_Rect camera_dst_rects[CAMERA_NUM] = {
  {1200,900,80,60}
};

// 敵が最初に向いている方向を指定する
static int enemy_lookangles[ENEMY_NUM] = {
  90,270
};
static int enemy_moveangles[ENEMY_NUM] = {
	90,270
};

/*  構造体の実体化  */
inputkeys key; //inputkeys構造体をinputという名前で実体化
playerinfo player[PLAYER_NUM];  // プレイヤーの情報を格納したplayer構造体を実体化
camerainfo camera[CAMERA_NUM];
enemyinfo enemy[ENEMY_NUM];
static objectinfo kotei_objects[KOTEI_OBJECT_NUM]; // 金塊、カメラ、棚、出入り口の動かない画面に固定のオブジェクトたちの情報を格納した「kotei_objects」という実体を作る

/* 関数プロトタイプ宣言 */
extern int InitObjectFromMap(int index, objecttype loadmap_objecttype, SDL_Rect dst); // マップデータを読み込んでその位置にオブジェクトを初期化する関数
extern void Imageload(void); //画像読み込み関数

#endif
