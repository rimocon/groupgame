#include <netinet/in.h>

#define DEFAULT_PORT 51000 //デフォルトのポート番号51000
#define MAX_LEN_NAME 10 //クライアントのユーザ名の最大長
#define MAX_NUM_CLIENTS 5 //接続要求の受付最大数
#define MAX_LEN_BUFFER 256 //メッセージの最大文字数
#define MAX_LEN_ADDR 32 //目盛りの大きさ
#define BROADCAST -1 //ブロードキャスト

#define MESSAGE_COMMAND 'M' //メッセージの送信
#define ZAHYO_COMMAND 'Z' //座標の送信
#define KINKAI_COMMAND 'K' //金塊の状態
#define HACK_COMMAND 'H' //ハッキング状態
#define HACK_START_COMMAND 'V' //ハッキング状態
#define NOT_HACK_COMMAND 'N' //ハッキングキャンセル
#define PLAYER_COMMAND 'P' //プレイヤーが死んだことを示す
#define UP_COMMAND 'U' //上へのスティック操作が行われたことを示す
#define DOWN_COMMAND 'D' //下へのスティック操作が行われたことを示す
#define RIGHT_COMMAND 'R' //右へのスティック操作が行われたことを示す
#define LEFT_COMMAND 'L' //左へのスティック操作が行われたことを示す
#define CENTER_COMMAND 'C' //真ん中(左右)へのスティック操作が行われたことを示す
#define AENTER_COMMAND 'A' //真ん中(上下)へのスティック操作が行われたことを示す
#define X_ON_COMMAND 'X'
#define X_OFF_COMMAND 'Y'
#define QUIT_COMMAND 'Q' //チャットシステムの終了
#define START_COMMAND 'S' //メニュー画面で、スタートボタンを押したことを表す
#define ENEMY_MODIFY_COMMAND 'B'
#define TALK_START_COMMAND 'T'
#define TALK_END_COMMAND 't'
#define CONTACT_COMMAND 'c'

/*  関数のプロトタイプ宣言 */
//またあとで

/*  構造体の宣言  */
typedef struct {
  int cid;
  int sock;
  struct sockaddr_in addr;
  char name[MAX_LEN_NAME];
} CLIENT;

typedef struct {
  int cid;
  char command;
  char message[MAX_LEN_BUFFER];
  int zahyo_x;
  int zahyo_y;
  int enemy_zahyo_x[5]; //NPCのx座標
  int enemy_zahyo_y[5]; //NPCのy座標
  int move_angle[5];
  int prev_angle[5];
} CONTAINER;



