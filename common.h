//2重インクルード防止
#ifndef _COMMON_H_
#define _COMMON_H_

/*  C言語のヘッダファイル */

#include <stdio.h>   //標準
#include <stdlib.h>  //標準マクロなど(NULLとか)
#include <string.h>  //文字列を扱いやすくする
#include <time.h>	//時間用
#include <stdbool.h> //bool型を使うため

/*  SDL関連 */
#include <SDL2/SDL.h>				 //SDLを使う
#include <SDL2/SDL2_gfxPrimitives.h> //SDLで図形描画
#include <SDL2/SDL_ttf.h>			 //SDLで文字描画
#include <SDL2/SDL_image.h>			 //SDLで画像を扱う
#include <SDL2/SDL_mixer.h>			 //SDLで音楽を扱う

/*  関数のプロトタイプ宣言  */
extern void Startup(void);		//最初に呼び出される関数
extern void Destroy(void);		//SDL関連破棄する関数
extern void Input(void);		//入力関数
extern void Imageload(void);	//画像読み込み関数
extern void RenderWindow(void); //画像の描画(イベントが無い時の)
extern void MoveChara(void);	// キャラクター移動のための関数

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
//#define WINDOWWIDTH 780 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ
//#define WINDOWHEIGHT 460 //ウィンドウの高さ

#define PLAYER_NUM 2
#define PLAYER_SPEED 3

#define KINKAI_NUM 1
#define CAMERA_NUM 1
#define SHELF_NUM 1
#define ENTRANCE_NUM 1
#define KOTEI_OBJECT_NUM 4

#define ENEMY_NUM 2
#define ENEMY_SPEED 1

/*  グローバル変数  */
int status;		  //ゲームの現在の状態
bool run;		  //プログラムが動いてるかどうか
bool kinkai_flag; //金塊が配置されているかどうか

SDL_Window *mainwindow;		//メイン画面用
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Surface *background;	//背景用サーフェイス
SDL_Joystick *joystick;		//ジョイスティックを特定,利用するための構造体
SDL_Event inputevent;		//入力用

typedef enum {
	TYPE_KINKAI = 0,
	TYPE_SHELF = 1,
	TYPE_CAMERA = 2,
	TYPE_ENTRANCE = 3,
	TYPE_ENEMY = 4,
	TYPE_PLAYER = 5,
	TYPE_NUM = 7
} objecttype;

typedef struct
{
	objecttype type;
	SDL_Texture *image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
} objectinfo;

typedef struct
{
	objecttype type;
	SDL_Texture *image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
	int speed;
} playerinfo;

typedef struct
{
	int rotate_range;
	objecttype type;
	SDL_Texture *image_texture;
	SDL_Rect src_rect;
	SDL_Rect dst_rect;
	bool flag_kinkai;
	int speed;
} camerainfo;

typedef struct
{				 //キー入力用の構造体を型宣言
	Uint32 left, //左矢印
		right,   //右矢印
		up,		 //上矢印
		down,	//下矢印
		a;		 //4ボタン(決定ボタン)
} inputkeys;

typedef struct {
	int rotate_range; // 敵の回転速度
	objecttype type; // タイプ
	SDL_Texture * image_texture; // テクスチャ
	SDL_Rect src_rect; // 元画像の座標、範囲
	SDL_Rect dst_rect; // 出力先の座標、範囲
	bool flag_kinkai; // 金塊をとったかどうか
	int speed; //敵の移動速度
	int look_angle; // 敵が向いている方向(0度〜360度)、視野の描画する方法によるので仮
	bool isgodest; // 目的地まで行ってるかどうか
}enemyinfo; // 敵の構造体

// プレイヤーの情報を格納したplayer構造体を実体化
playerinfo player[PLAYER_NUM];
enemyinfo enemy[ENEMY_NUM];

#endif
