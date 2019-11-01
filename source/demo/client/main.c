#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../constants.h"
#include "../common.h"

extern void setup_client(char *, u_short);
extern int control_requests();
extern void terminate_client();

int main(int argc, char *argv[]) {
  u_short port = DEFAULT_PORT;
  char server_name[MAX_LEN_NAME];

  sprintf(server_name, "localhost");

  switch (argc) {
  case 1:
    break;
  case 2:
    sprintf(server_name, "%s", argv[1]);
    break;
  case 3:
    sprintf(server_name, "%s", argv[1]);
    port = (u_short)atoi(argv[2]);
    break;
  default:
    fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
    return 1;
  }

  setup_client(server_name, port);
  Startup(); //描画に関するセットアップ

  //int cond = 1;
  //while (cond){
  while(1){
    //cond = control_requests();
    if((SDL_PollEvent(&inputevent))){
      printf("o\n"); //キーボードの入力に反応しているか確認用
      Input();
    }
  }

  terminate_client();

  return 0;
}
