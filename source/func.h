/*  二重インクルード防止  */
#ifndef _FUNC_H_
#define _FUNC_H_

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define PLAYER_NUM 1
#define PLAYER_SPEED 1

#define KINKAI_NUM 1
#define CAMERA_NUM 1
#define SHELF_NUM 1
#define ENTRANCE_NUM 1
#define  KOTEI_OBJECT_NUM 4


/*  func.c内のグローバル変数  */
float tri[2][3] = { { 640, 590, 690},
                   { 480, 360, 360} };
bool clockwise;
float theta; //角度
float theta2; //角度
/*  構造体宣言  */
typedef enum{
	TYPE_KINKAI = 0,
	TYPE_SHELF = 1,
	TYPE_CAMERA = 2,
	TYPE_ENTRANCE = 3,
	TYPE_ENEMY = 4,
	TYPE_PLAYER = 5,
	TYPE_NUM = 6
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
	int rotate_range;
	objecttype type;
	SDL_Texture * image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
	int speed;
}camerainfo;

typedef struct { //キー入力用の構造体を型宣言
	Uint32  left, //左矢印
	 			  right, //右矢印
				  up, //上矢印
				  down, //下矢印
          a;  //4ボタン(決定ボタン)
}inputkeys;

/*  構造体の実体化  */

//画像ファイルパス
static char *imgfiles[TYPE_NUM] = {"./images/kinkai.png","./images/shelf.png","./images/camera.png","./images/entrance.png","./images/enemy.png","./images/player.png"}; // 読み込む画像ファイルを指定

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

inputkeys key; //inputkeys構造体をinputという名前で実体化
playerinfo player[PLAYER_NUM];  // プレイヤーの情報を格納したplayer構造体を実体化



#endif
