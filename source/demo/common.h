//2重インクルード防止
#ifndef _COMMON_H_
#define _COMMON_H_

/*  C言語のヘッダファイル */

#include <stdio.h> //標準
#include <stdlib.h> //標準マクロなど(NULLとか)
#include <string.h> //文字列を扱いやすくする
#include <time.h> //時間用
#include <stdbool.h> //bool型を使うため

/*  SDL関連 */
#include <SDL2/SDL.h> //SDLを使う
#include <SDL2/SDL2_gfxPrimitives.h> //SDLで図形描画
#include <SDL2/SDL_ttf.h> //SDLで文字描画
#include <SDL2/SDL_image.h> //SDLで画像を扱う
#include <SDL2/SDL_mixer.h> //SDLで音楽を扱う

/*  関数のプロトタイプ宣言  */
extern void Startup(void); //最初に呼び出される関数
extern void Destroy(void); //SDL関連破棄する関数
extern void Input(void); //入力関数
extern void Imageload(void); //画像読み込み関数
extern void RenderWindow(void); //画像の描画(イベントが無い時の)
extern int coord(void); //座標に関する関数

/*  グローバル変数  */
int status; //ゲームの現在の状態
bool run; //プログラムが動いてるかどうか
bool flag_kinkai; //金塊が置かれている時'true'、取られた時'false'

int circle_x, circle_y; //プレイヤーの位置(現在は、簡略化のため、赤点)

SDL_Window *mainwindow; //メイン画面用
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Surface *background; //背景用サーフェイス
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
SDL_Event inputevent; //入力用
SDL_Rect src_rect_kinkai; // 転送元画像の領域
SDL_Rect dst_rect_kinkai; // 転送先画像の領域
SDL_Rect src_rect_shelf;  // 転送元画像の領域
SDL_Rect dst_rect_shelf;  // 転送先画像の領域
SDL_Texture *texture_kinkai; //金塊のテキスチャ
SDL_Texture *texture_shelf;  //棚のテクスチャ

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define PLAYER_NUM 1
#define PLAYER_SPEED 3

#define KINKAI_NUM 1
#define CAMERA_NUM 1
#define SHELF_NUM 1
#define ENTRANCE_NUM 1
#define  KOTEI_OBJECT_NUM 4

/*  グローバル変数  */
int status; //ゲームの現在の状態
bool run; //プログラムが動いてるかどうか

SDL_Window *mainwindow; //メイン画面用
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Surface *background; //背景用サーフェイス
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
SDL_Event inputevent; //入力用

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
	Uint32        left, //左矢印
	 			  right, //右矢印
				  up, //上矢印
				  down, //下矢印
                  a;  //4ボタン(決定ボタン)
}inputkeys;

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ
#endif
