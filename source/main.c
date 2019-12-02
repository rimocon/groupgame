#include "../common.h"
#include "../constants.h"
static Uint32 AniTimer(Uint32 interval, void *param);

int main(int argc, char *argv[])
{

/*  フレームレート用の変数  */
  const int fps = 60.0; //1秒に何回描画するか
  const int framedelay = 1000.0 / fps; //1秒 = 1000msなので1回の描画にかけるべき時間
  Uint32 framestart; //処理の始まりの時間を格納する変数
  int frametime; //1回の処理にかかった時間を格納する変数

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
  Startup(); //初期設定

    while (run) {
    framestart = SDL_GetTicks();
    if((SDL_PollEvent(&inputevent)))
    {
      Input(); 
      printf("input\n");
    }
    control_requests();
    switch(status){
      case MENUMODE:
        DrawMenu();
        break;
      case GAMEMODE:
        MoveChara(); //$B%-%c%i0\F0(B
        MoveTriangle(); //$B;03Q7A0\F0(B
        Collision(); //$BEv$?$jH=Dj(B
        RenderWindow(); //$BIA2h(B
        //Destroy(); //$BGK4~4XO"(B
        //SDL_Delay(3);
        break;
      case RESULTMODE:
        //DrawResult();
        break;
    }
    frametime = SDL_GetTicks() - framestart; //処理が終わった時間-処理が始まった時間=1回のループ処理にかかった時間
    printf("一回の処理時間　%d\n",frametime);
    if(framedelay > frametime) //一回の処理にかける時間より1回のループ処理にかかった時間が小さかったら
    {
      SDL_Delay(framedelay - frametime); //余った時間分おやすみ
    }
  }
  terminate_client(); //ソケットの切断
  return 0;
}
