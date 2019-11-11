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
extern void Imageload(void); //画像読み込み関数
extern void RenderWindow(void); //画像の描画(イベントが無い時の)
extern void MoveChara(void); // キャラクター移動のための関数
<<<<<<< HEAD
extern void MoveTriangle(void); //三角形移動
extern void Collision(); //当たり判定

/*  define関連  */
#define WINDOWWIDTH 1280 //ウィンドウの幅
#define WINDOWHEIGHT 960 //ウィンドウの高さ

#define PLAYER_NUM 1 // オブジェクトの数などはテキストファイルで読み込めるようにしたほうがいろんなマップに対応できるから後から修正したい
#define PLAYER_SPEED 3

#define KINKAI_NUM 1
#define CAMERA_NUM 1
#define SHELF_NUM 1
#define ENTRANCE_NUM 1
#define  KOTEI_OBJECT_NUM 4 // KINKAI_NUM + CAMERA_NUM + SHELF_NUM + ENTRANCE_NUMを足したもの

#define ENEMY_NUM 2
#define ENEMY_SPEED 1

#define MAP_CHIPSIZE 40 //仮
#define MAP_WIDTH 32
#define MAP_HEIGHT 24

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
}objecttype; // オブジェクトのタイプと数字を関連付け、オブジェクトの構造体が何であるか判別

typedef struct {
	objecttype type; // オブジェクトのタイプ
	SDL_Texture *image_texture; // テクスチャ
	SDL_Rect src_rect; // 元画像の座標、範囲
	SDL_Rect dst_rect; // 出力先の座標、範囲
} objectinfo; // （ゲーム中動くことがない）オブジェクト

typedef struct {
	objecttype type;
	SDL_Texture * image_texture; // テクスチャ
	SDL_Rect src_rect; // 元画像の座標、範囲
	SDL_Rect dst_rect; // 出力先の座標、範囲
	bool flag_kinkai; // 金塊をとったかどうか
	int speed; // プレイヤーの移動速度
	bool flag_collision;
	SDL_Rect prev_dst_rect;
}playerinfo; // プレイヤーの構造体


typedef struct {
	int rotate_range; // カメラの回転速度
	objecttype type; // タイプ
	SDL_Texture * image_texture; // テクスチャ
	SDL_Rect src_rect; // 元画像の座標、範囲
	SDL_Rect dst_rect; // 出力先の座標、範囲
	bool flag_kinkai; // 金塊をとったかどうか
	int speed; // カメラの回転スピード
}camerainfo; // カメラの構造体


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


typedef struct { //キー入力用の構造体を型宣言
	Uint32  left, //左矢印
	 			  right, //右矢印
				  up, //上矢印
				  down, //下矢印
          a;  //4ボタン(決定ボタン)
}inputkeys;

#endif
