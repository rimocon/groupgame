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

/*  ネット関連 */
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>


/*  SDL関連 */
#include <SDL2/SDL.h> //SDLを使う
#include <SDL2/SDL2_gfxPrimitives.h> //SDLで図形描画
#include <SDL2/SDL_ttf.h> //SDLで文字描画
#include <SDL2/SDL_image.h> //SDLで画像を扱う
#include <SDL2/SDL_mixer.h> //SDLで音楽を扱う

#define MENUMODE 1
#define GAMEMODE 2
#define RESULTMODE 3

/*  関数のプロトタイプ宣言  */

extern void Startup(void); //最初に呼び出される関数
extern void DrawMenu(void); //メニュー画面描画
extern void Destroy(void); //SDL関連破棄する関数
extern void Input(void); //入力関数
extern void RenderWindow(void); //画像の描画(イベントが無い時の)
extern void MoveChara(void); // キャラクター移動のための関数
extern void MoveTriangle(void); //三角形移動
extern void Collision(); //当たり判定
extern void MakeMap(void);
extern void DrawResult(void); //結果描画
extern void PlayerAction(); // プレイヤーが行う妨害アクション
extern void Events(void); //イベント処理

extern void setup_client(char *, u_short);
extern int control_requests();
extern void terminate_client();
extern void joystick_send(int);

/*  グローバル変数  */
int status;          //ゲームの現在の状態
bool run;            //プログラムが動いてるかどうか
int elapsed_time;    //ゲーム開始時からの経過時間
bool same_place_flag; //NPCが一定時間同じ座標にとどまっているかどうかを示すフラグ
bool random_start_flag; //NPCが一定時間同じ座標にとどまっため、ランダムウォークを始めたことを示すフラグ
Uint32 stay_start;   //NPCが同じ場所に留まり始めた時間を格納する変数
int stay_time;       //留まっている時間を格納する変数
int random_time;     //ランダムウォークをしている時間
int myid;

SDL_Window *mainwindow; //メイン画面用
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
SDL_Event inputevent; //入力用
#endif
