#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "../constants.h"
#include "../common.h"

inputkeys key; //inputkeys構造体をinputという名前で実体化

static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask;
int speaker; //発言者の番号
static CLIENT clients[MAX_NUM_CLIENTS];

void setup_client(char *, u_short);
int control_requests();
void terminate_client();

static int input_command(void);
static int execute_command(void);
static void send_data(void *, int);
static int receive_data(void *, int);
static void handle_error(char *);

void Startup() //描画に関する最初のセットアップを行う
{

  circle_x = 150, circle_y = 850; //点の初期位置
}
void setup_client(char *server_name, u_short port)
{
  struct hostent *server;
  struct sockaddr_in sv_addr;

  fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
  if ((server = gethostbyname(server_name)) == NULL)
  {
    handle_error("gethostbyname()");
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    handle_error("socket()");
  }

  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(port);
  sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0];

  if (connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
  {
    handle_error("connect()");
  }

  fprintf(stderr, "Input your name: ");
  char user_name[MAX_LEN_NAME];
  if (fgets(user_name, sizeof(user_name), stdin) == NULL)
  {
    handle_error("fgets()");
  }
  user_name[strlen(user_name) - 1] = '\0';
  send_data(user_name, MAX_LEN_NAME);

  fprintf(stderr, "Waiting for other clients...\n");
  receive_data(&num_clients, sizeof(int)); //クライアントの数
  fprintf(stderr, "Number of clients = %d.\n", num_clients);
  receive_data(&myid, sizeof(int)); //クライアントID
  fprintf(stderr, "Your ID = %d.\n", myid);
  receive_data(&speaker, sizeof(int)); //発言者の情報の読み込み

  int i;
  //クライアントのデータを読み込み
  for (i = 0; i < num_clients; i++)
  {
    receive_data(&clients[i], sizeof(CLIENT));
  }

  fprintf(stderr, "現在の発言者は，　%s　さんです.\n", clients[speaker].name); //現在の発言者を表示

  num_sock = sock + 1;
  FD_ZERO(&mask);
  FD_SET(0, &mask);
  FD_SET(sock, &mask);
}

void Input()
{
  switch (inputevent.type)
  {
  case SDL_KEYDOWN: // キーボードのキーが押された時
    // 押されたキーの名前等を表示
    printf("The pressed key is %s.\n", SDL_GetKeyName(inputevent.key.keysym.sym));
    printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), inputevent.key.keysym.mod);
    // 押されたキーごとに処理
    switch (inputevent.key.keysym.sym)
    {
    case SDLK_UP:
      printf("press up\n");
      key.right = 0;
      key.left = 0;
      key.up = 1;
      key.down = 0;
      circle_y = circle_y - 10; //プレイヤーの座標を上方向に変更
      break;
    case SDLK_DOWN:
      printf("press down\n");
      key.right = 0;
      key.left = 0;
      key.up = 0;
      key.down = 1;
      circle_y = circle_y + 10; //プレイヤーの座標を下方向に変更
      break;
    case SDLK_RIGHT:
      printf("press right\n");
      key.right = 1;
      key.left = 0;
      key.up = 0;
      key.down = 0;
      circle_x = circle_x + 10; //プレイヤーの座標を右方向に変更
      break;
    case SDLK_LEFT:
      printf("press left\n");
      key.right = 0;
      key.left = 1;
      key.up = 0;
      key.down = 0;
      circle_x = circle_x - 10; //プレイヤーの座標を左方向に変更
      break;
    case SDLK_1:
      run = false;
    case SDLK_SPACE: //金塊を取るボタン
      if (circle_x > 1000 && circle_x < 1100 && circle_y > 100 && circle_y < 200)
      {
        flag_kinkai = false;
      }
      break;
    }
    break;
  }
  printf("%d, %d\n", circle_x, circle_y); //プレイヤーの座標確認用
}

int control_requests()
{
  fd_set read_flag = mask;

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 30;

  if (select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1)
  {
    handle_error("select()");
  }

  int result = 1;
  //クライアント自身がコマンドを送る時
  if (FD_ISSET(0, &read_flag))
  {
    result = input_command();
    //他のクライアントからコマンドを受信した時
  }
  else if (FD_ISSET(sock, &read_flag))
  {
    result = execute_command();
  }

  return result;
}

static int input_command()
{
  CONTAINER data;
  char com;
  memset(&data, 0, sizeof(CONTAINER));
  com = getchar();
  while (getchar() != '\n')
    ;

  switch (com)
  {
  case MESSAGE_COMMAND:
    fprintf(stderr, "Input message: ");
    if (fgets(data.message, MAX_LEN_BUFFER, stdin) == NULL)
    {
      handle_error("fgets()");
    }
    data.command = MESSAGE_COMMAND;
    data.message[strlen(data.message) - 1] = '\0';
    data.cid = myid;
    send_data(&data, sizeof(CONTAINER));
    break;
  case QUIT_COMMAND:
    data.command = QUIT_COMMAND;
    data.cid = myid;
    send_data(&data, sizeof(CONTAINER));
    break;
  default:
    fprintf(stderr, "%c is not a valid command.\n", com);
  }

  return 1;
}

static int execute_command()
{
  CONTAINER data;
  int result = 1;

  memset(&data, 0, sizeof(CONTAINER));
  receive_data(&data, sizeof(data));

  switch (data.command)
  {
  case MESSAGE_COMMAND:
    fprintf(stderr, "client[%d] %s: %s\n", data.cid, clients[data.cid].name, data.message);
    result = 1;
    fprintf(stderr, "現在の発言者は，　%s　さんです.\n", clients[data.speaker].name); //現在の発言者を表示
    break;
  case QUIT_COMMAND:
    fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
    result = 0;
    break;
  case WARNING_COMMAND: //発言権が無い人が発言した場合
    fprintf(stderr, "%s\n", data.warning_message);
    break;
  default:
    fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);
    exit(1);
  }
  return result;
}

static void send_data(void *data, int size)
{
  if ((data == NULL) || (size <= 0))
  {
    fprintf(stderr, "send_data(): data is illeagal.\n");
    exit(1);
  }

  if (write(sock, data, size) == -1)
  {
    handle_error("write()");
  }
}

static int receive_data(void *data, int size)
{
  if ((data == NULL) || (size <= 0))
  {
    fprintf(stderr, "receive_data(): data is illeagal.\n");
    exit(1);
  }

  return (read(sock, data, size));
}

static void handle_error(char *message)
{
  perror(message);
  fprintf(stderr, "%d\n", errno);
  exit(1);
}

void terminate_client()
{
  fprintf(stderr, "Connection is closed.\n");
  close(sock);
  exit(0);
}
