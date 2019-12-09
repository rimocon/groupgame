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
#define PLAYER_COMMAND 'P' //プレイヤーが死んだことを示す
#define UP_COMMAND 'U' //上へのスティック操作が行われたことを示す
#define DOWN_COMMAND 'D' //下へのスティック操作が行われたことを示す
#define RIGHT_COMMAND 'R' //右へのスティック操作が行われたことを示す
#define LEFT_COMMAND 'L' //左へのスティック操作が行われたことを示す
#define CENTER_COMMAND 'C' //真ん中(左右)へのスティック操作が行われたことを示す
#define AENTER_COMMAND 'A' //真ん中(上下)へのスティック操作が行われたことを示す
#define QUIT_COMMAND 'Q' //チャットシステムの終了

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
} CONTAINER;



