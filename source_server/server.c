#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../constants.h"

static CLIENT clients[MAX_NUM_CLIENTS];
static int num_clients;
static int num_socks;
static fd_set mask; //FD集合を表す構造体
static CONTAINER data;

void setup_server(int, u_short);
int control_requests();
void terminate_server();

static void send_data(int, void *, int);
static int receive_data(int, void *, int);
static void handle_error(char *);

/*
サーバのセットアップを行う関数でクライアントの数とポート番号を引数とする
u_short型は、unsigned short intのことで、値の範囲は、0 ～ 65,535
*/
void setup_server(int num_cl, u_short port)
{
  int rsock, sock = 0; //ソケットの識別番号を0で初期化
  /*
  sockaddr_in構造体は、接続先のIPアドレスやポート番号の情報を保持するためのものである。
  各ソケットは，bindシステムコールによって sockaddr_in構造体のデータと関連づけられる
  */
  struct sockaddr_in sv_addr, cl_addr;
  //標準エラー出力でサーバのセットアップが開始されたことを示す。
  fprintf(stderr, "Server setup is started.\n");

  num_clients = num_cl; //クライアントを代入

  //接続要求受付用ソケットの生成 (サーバ側)
  //引数1：ネットワークアドレスの種類、ここではインターネット
  //引数2：ソケットの種類、ここではストリームソケット(TCP用)
  //引数3：プロトコル、ここでは自動選択をしているため、TCPが選ばれる
  rsock = socket(AF_INET, SOCK_STREAM, 0); //戻り値は、ソケットトークン(ソケットの識別番号)
  if (rsock < 0)
  {                           //正常に実行できなかった場合
    handle_error("socket()"); //エラーを表示
  }
  //実行に成功したことを表示
  fprintf(stderr, "sock() for request socket is done successfully.\n");

  sv_addr.sin_family = AF_INET;         //ネットワークアドレスの種類(ここでは、インターネット)
  sv_addr.sin_port = htons(port);       //ポート番号
  sv_addr.sin_addr.s_addr = INADDR_ANY; //IPアドレス

  int opt = 1;
  /*
  ファイルディスクリプターsocketで参照されるソケットに関連するオプションの操作を行う
  引数3,4は、setsockopt() のオプションの値にアクセスするために用いられる
  */
  setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(rsock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
  {                         //ソケットに設定を結びつける
    handle_error("bind()"); //エラーを表示
  }
  //実行に成功したことを表示
  fprintf(stderr, "bind() is done successfully.\n");

  if (listen(rsock, num_clients) != 0)
  {                           //接続ソケットの準備，引数1は，接続要求受付用ソケット
    handle_error("listen()"); //エラーを表示
  }
  fprintf(stderr, "listen() is started.\n");

  int i, max_sock = 0;
  socklen_t len; //accept()で使用される、アドレスの長さを格納する型
  char src[MAX_LEN_ADDR];
  for (i = 0; i < num_clients; i++)
  {
    len = sizeof(cl_addr);
    /*接続ソケットの生成と接続の確立
    戻り値：新しいソケットのディスクリプタであるSOCKET型の値
    
    acceptシステムコールについて詳しく
    この関数は、保留状態の接続要求が入っているキューから先頭の接続要求を取り出し、接続済みソケットを新規に生成し、
    そのソケットを参照する新しいファイル・ディスクリプタを返す。新規に生成されたソケットは，接続待ち(listen)状態ではない．
    もともとのソケット sockfd はこの呼び出しによって影響を受けない。*/
    sock = accept(rsock, (struct sockaddr *)&cl_addr, &len);
    if (sock < 0)
    {                           //正常に実行できなかった場合
      handle_error("accept()"); //エラー表示
    }
    if (max_sock < sock)
    {
      max_sock = sock;
    }
    //コネクションからバファへのデータの読み込み(ここでは名前の読み込み)
    /*
    read関数
    引数1:接続ソケット
    引数2:読み込み先のバッファ
    ここでの読み込み先バッファは、CLIENT構造体のname
    引数3:読み込みバイト数
    */
    if (read(sock, clients[i].name, MAX_LEN_NAME) == -1)
    {
      handle_error("read()");
    }
    clients[i].cid = i; //クライアントを識別するためのIDで、サーバーへの接続順で0から順に割り振られる
    clients[i].sock = sock;
    clients[i].addr = cl_addr;
    /*
    memset関数
    メモリに指定バイト数分の値をセットすることができる
    第一引数はメモリのポインタ
    第二引数はセットする値
    第三引数はセットするサイズ
    */
    memset(src, 0, sizeof(src));
    /*
    inet_ntop関数
    引数2で指定された、バイナリー形式のIP アドレスを、標準テキスト形式に変換し、その結果を src に入れる。
    引数1は、IP アドレスのファミリーを指定します。これは、AF_INET または AF_INET6 が可能です。
    */
    inet_ntop(AF_INET, (struct sockaddr *)&cl_addr.sin_addr, src, sizeof(src));
    //ntohs関数は、短整数をネットワーク・バイト・オーダーからホスト・バイト・オーダーへ変換する
    fprintf(stderr, "Client %d is accepted (name=%s, address=%s, port=%d).\n", i, clients[i].name, src, ntohs(cl_addr.sin_port));
  }
  close(rsock); //接続要求受付用ソケットを切断

  int j;
  for (i = 0; i < num_clients; i++)
  {
    //send_data関数:引数1がクライアントを識別するためのID
    send_data(i, &num_clients, sizeof(int)); //クライアント数
    send_data(i, &i, sizeof(int));           //クライアントID
    for (j = 0; j < num_clients; j++)
    {
      send_data(i, &clients[j], sizeof(CLIENT)); //クライアントのデータ
    }
  }

  num_socks = max_sock + 1;
  FD_ZERO(&mask); //&maskをゼロクリア
  FD_SET(0, &mask);

  for (i = 0; i < num_clients; i++)
  {
    FD_SET(clients[i].sock, &mask); //clients[i].sock番目のFDに対応する値を1にセット
  }
  fprintf(stderr, "Server setup is done.\n");
}

int control_requests()
{
  fd_set read_flag = mask;
  memset(&data, 0, sizeof(CONTAINER)); //dataの初期化

  fprintf(stderr, "select() is started.\n");
  //3 つの独立したファイルディスクリプタ集合の監視を行う。
  if (select(num_socks, (fd_set *)&read_flag, NULL, NULL, NULL) == -1)
  {
    handle_error("select()");
  }

  int i, result = 1;
  for (i = 0; i < num_clients; i++)
  {
    /*
    FD_ISSET関数
    clients[i].sock番目のファイルディスクリプタが1かどうか確認
    */
    if (FD_ISSET(clients[i].sock, &read_flag))
    {
      receive_data(i, &data, sizeof(data)); //クライアントのデータを受信
      switch (data.command)
      {
      case ZAHYO_COMMAND: //'Z'のとき
        fprintf(stderr, "client[%d], name : %s, zahyo_x = %d, zahyo_y = %d \n", clients[i].cid, clients[i].name, data.zahyo_x, data.zahyo_y);
        send_data(BROADCAST, &data, sizeof(data));
        result = 1;
        break;
      case KINKAI_COMMAND: //'K'のとき
        fprintf(stderr, "client[%d], name : %s, get kinkai !!!!! \n", clients[i].cid, clients[i].name);
        send_data(BROADCAST, &data, sizeof(data));
        result = 1;
        break;
      case MESSAGE_COMMAND: //'M'のとき
        fprintf(stderr, "client[%d] %s: message = %s\n", clients[i].cid, clients[i].name, data.message);
        send_data(BROADCAST, &data, sizeof(data));
        result = 1;
        break;
      case QUIT_COMMAND: //'Q'のとき
        fprintf(stderr, "client[%d] %s: quit\n", clients[i].cid, clients[i].name);
        send_data(BROADCAST, &data, sizeof(data));
        result = 0;
        break;
      default: //その他の文字が入力された場合
        fprintf(stderr, "control_requests(): %c is not a valid command.\n", data.command);
        exit(1); //異常終了
      }
    }
  }

  return result; //戻り値は0か1で，これを元にループするか，抜けるか判断する
}

static void send_data(int cid, void *data, int size)
{
  //ユニキャストで，かつクライアント数が範囲内のとき
  if ((cid != BROADCAST) && (0 > cid || cid >= num_clients))
  {
    fprintf(stderr, "send_data(): client id is illeagal.\n");
    exit(1);
  }
  //CONTAINER構造体で宣言されたdataに不具合がある場合
  if ((data == NULL) || (size <= 0))
  {
    fprintf(stderr, "send_data(): data is illeagal.\n");
    exit(1);
  }

  if (cid == BROADCAST)
  { //送信先がすべてのクライアントのとき
    int i;
    for (i = 0; i < num_clients; i++)
    { //すべてのクライアントに対して，クライアントデータを送信
      if (write(clients[i].sock, data, size) < 0)
      {
        handle_error("write()");
      }
    }
  }
  else
  {
    /*
    wrire関数
    write() は、 引数2が指すバッファーから、ファイルディスクリプター引数1が
    参照するファイルへ、最大引数3のバイト数分を書き込む
    */
    if (write(clients[cid].sock, data, size) < 0)
    { //送信先が指定されているときは，指定のクライアントにクライアントデータを送信
      handle_error("write()");
    }
    fprintf(stderr, "cid = %d\n", cid);
  }
}

static int receive_data(int cid, void *data, int size)
{
  //ユニキャストで，かつクライアント数が範囲内のとき
  if ((cid != BROADCAST) && (0 > cid || cid >= num_clients))
  {
    fprintf(stderr, "receive_data(): client id is illeagal.\n");
    exit(1);
  }
  //CONTAINER構造体で宣言されたdataに不具合がある場合
  if ((data == NULL) || (size <= 0))
  {
    fprintf(stderr, "receive_data(): data is illeagal.\n");
    exit(1);
  }
  //read関数 : コネクションからバッファへのデータの読み込み
  return read(clients[cid].sock, data, size);
}

static void handle_error(char *message)
{
  //標準エラー出力に errno のエラー番号に対応するエラーメッセージを書き込む
  perror(message);
  fprintf(stderr, "%d\n", errno);
  exit(1); //異常終了
}

void terminate_server(void)
{
  int i;
  for (i = 0; i < num_clients; i++)
  {
    close(clients[i].sock); //全クライアントのソケットを切断
  }
  fprintf(stderr, "All connections are closed.\n");
  exit(0); //正常終了
}
