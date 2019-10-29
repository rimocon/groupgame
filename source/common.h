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

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define IMAGE_NUM 2 // 読み込む画像の総数を指定

/*  グローバル変数  */
int status; //ゲームの現在の状態
bool run; //プログラムが動いてるかどうか

SDL_Window *mainwindow; //メイン画面用
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Surface *background; //背景用サーフェイス
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
SDL_Event inputevent; //入力用

static char *imagefiles[IMAGE_NUM] = {"./images/kinkai.png","./images/shelf.png"}; // 読み込む画像ファイルを指定
static SDL_Rect images_dst_rects[IMAGE_NUM] = {{1000, 100, 100, 100}, {400, 100, 46, 108}}; // imagefilesの出力する領域を設定


typedef struct { //キー入力用の構造体を型宣言
	Uint32  left, //左矢印
	 			  right, //右矢印
				  up, //上矢印
				  down, //下矢印
          a;  //4ボタン(決定ボタン)
}inputkeys;


#endif
