#include <netinet/in.h>

#define DEFAULT_PORT 51000
#define MAX_LEN_NAME 10
#define MAX_NUM_CLIENTS 5
#define MAX_LEN_BUFFER 256
#define MAX_LEN_ADDR 32
#define BROADCAST -1

#define MESSAGE_COMMAND 'M'
#define ZAHYO_COMMAND 'Z' //座標を表すコマンド
#define QUIT_COMMAND 'Q'
/*発言順番が間違っている場合，サーバー側が構造体のcomandに
　警告を表す'W'を代入して，クライアント送信*/
#define WARNING_COMMAND 'W' 

typedef struct {
  int cid;
  int sock;
  //int speaker; //発言者
  struct sockaddr_in addr;
  char name[MAX_LEN_NAME];
} CLIENT;

typedef struct {
  int cid;
  char command;
  char message[MAX_LEN_BUFFER];
  int speaker; //発言者番号
  char warning_message[MAX_LEN_BUFFER]; //警告文を送信用
} CONTAINER;

typedef struct{
  int cid;
  int controller; //座標をコントロールした人の番号
} ZAHYO;

extern int speaker; //発言者の番号
extern char warning_temp[MAX_LEN_BUFFER]; //警告文
