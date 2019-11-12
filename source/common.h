//2重インクルード防止
#ifndef _COMMON_H_
#define _COMMON_H_

/*  C言語のヘッダファイル */

#include <stdio.h> //標準
#include <stdlib.h> //標準マクロなど(NULLとか)
#include <string.h> //文字列を扱いやすくする
#include <time.h> //時間用
#include <stdbool.h> //bool型を使うため
#include <math.h> //数学用ライブラリ
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
extern void RenderWindow(void); //画像の描画(イベントが無い時の)
extern void MoveChara(void); // キャラクター移動のための関数
extern void MoveTriangle(void); //三角形移動
extern void Collision(); //当たり判定
extern void MakeMap(void);

/*  グローバル変数  */
int status; //ゲームの現在の状態
bool run; //プログラムが動いてるかどうか
SDL_Window *mainwindow; //メイン画面用
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Surface *background; //背景用サーフェイス
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
SDL_Event inputevent; //入力用
#endif
