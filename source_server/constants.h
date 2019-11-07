#include <netinet/in.h>

#define DEFAULT_PORT 51000 //デフォルトのポート番号51000
#define MAX_LEN_NAME 10 //クライアントのユーザ名の最大長
#define MAX_NUM_CLIENTS 5 //接続要求の受付最大数
#define MAX_LEN_BUFFER 256 //メッセージの最大文字数
#define MAX_LEN_ADDR 32 //目盛りの大きさ
#define BROADCAST -1 //ブロードキャスト

#define MESSAGE_COMMAND 'M' //メッセージの送信
#define QUIT_COMMAND 'Q' //チャットシステムの終了

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
} CONTAINER;
