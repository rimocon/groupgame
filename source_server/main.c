#include <stdio.h>
#include <stdlib.h>
#include "../constants.h"

extern void setup_server(int, u_short);
extern int control_requests();
extern void terminate_server();

int main(int argc, char *argv[]) {
  int num_cl = 1;
  u_short port = DEFAULT_PORT;

  switch (argc) {
  case 1: //コマンドライン引数の数が1つの場合 
    break; //変数は更新されず，初期化の値がそのまま使われる
  case 2: //コマンドライン引数の数が2つの場合
    num_cl = atoi(argv[1]); //クライアント数のみ更新
    break;
  case 3: //コマンドライン引数の数が3つの場合 
    num_cl = atoi(argv[1]); //クライアント数を更新
    port = atoi(argv[2]); //ポート番号を更新
    break;
  default: //コマンドライン引数の数が4以上のとき
    fprintf(stderr, "Usage: %s [number of clients] [port number]\n", argv[0]); //入力方法を表示
    return 1;
  }

  //指定したクライアントの数が接続要求の受付最大数を超えている場合
  if (num_cl < 0 || num_cl > MAX_NUM_CLIENTS) {
    //エラー文を表示
    fprintf(stderr, "Max number of clients is %d\n", MAX_NUM_CLIENTS);
    return 1;
  }

  fprintf(stderr, "Number of clients = %d\n", num_cl); //クライアントの数を表示
  fprintf(stderr, "Port number = %d\n", port); //ポート番号の表示

  setup_server(num_cl, port); //サーバのセットアップを行う関数，引数として、クライアントの数とポート番号

  int cond = 1;
  while (cond) { //関数control_requestsの戻り値が，0になるまでループ
      cond = control_requests(); 
  }

  terminate_server(); //ソケットの切断

  return 0;
}
