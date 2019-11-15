#include "../common.h"
#include "../constants.h"
#include "func.h"

extern void setup_client(char *, u_short);
extern int control_requests();
extern void terminate_client();
extern void joystick_send(int);

int main (int argc, char *argv[]) {

  u_short port = DEFAULT_PORT;
  char server_name[MAX_LEN_NAME];

  sprintf(server_name, "localhost");

  switch (argc) {
  case 1: //コマンドライン引数の数が1つの場合
    break;//変数は更新されず，初期化の値がそのまま使われる
  case 2: //コマンドライン引数の数が2つの場合
    sprintf(server_name, "%s", argv[1]); //名前を格納
    break; 
  case 3://コマンドライン引数の数が3つの場合
    sprintf(server_name, "%s", argv[1]); //名前を格納
    port = (u_short)atoi(argv[2]); //ポート番号の更新
    break;
  default: //コマンドライン引数が4つ以上の場合
    fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
    return 1;
  }

  setup_client(server_name, port); //クライアントのセットアップを行う関数

  Startup(); //$B:G=i$NFI$_9~$_4XO"(B
  while (run) {
    if((SDL_PollEvent(&inputevent))){
      Input(); //$BF~NO(B
    }
    MoveTriangle(); //$B;03Q7A0\F0(B
    Collision(); //$BEv$?$jH=Dj(B
    MoveChara(); //$B%-%c%i0\F0(B
    control_requests();
    SDL_Delay(3);
    RenderWindow();
    printf("%d\n",camera[0].tri[0][0]);
    //SDL_RenderPresent(mainrenderer);
  }
  terminate_client(); //ソケットの切断
  return 0;
}

