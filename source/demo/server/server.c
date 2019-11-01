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
static fd_set mask;
static CONTAINER data;

void setup_server(int, u_short);
int control_requests();
void terminate_server();

static void send_data(int, void *, int);
static int receive_data(int, void *, int);
static void handle_error(char *);

int speaker = 0; //最初の発言者は，クライアント番号0
char warning_temp[MAX_LEN_BUFFER] = "It's not your turn!!!";

void setup_server(int num_cl, u_short port) {
  int rsock, sock = 0;
  struct sockaddr_in sv_addr, cl_addr;

  fprintf(stderr, "Server setup is started.\n");

  num_clients = num_cl;

  rsock = socket(AF_INET, SOCK_STREAM, 0);
  if (rsock < 0) handle_error("socket()");
  fprintf(stderr, "sock() for request socket is done successfully.\n");

  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(port);
  sv_addr.sin_addr.s_addr = INADDR_ANY;

  int opt = 1;
  setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(rsock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) {
    handle_error("bind()");
  }
  fprintf(stderr, "bind() is done successfully.\n");

  if (listen(rsock, num_clients) != 0) {
    handle_error("listen()");
  }
  fprintf(stderr, "listen() is started.\n");

  int i, max_sock = 0;
  socklen_t len;
  char src[MAX_LEN_ADDR];
  for (i = 0; i < num_clients; i++) {
    len = sizeof(cl_addr);
    sock = accept(rsock, (struct sockaddr *)&cl_addr, &len);
    if (sock < 0) {
      handle_error("accept()");
    }
    if (max_sock < sock) {
      max_sock = sock;
    }
    if (read(sock, clients[i].name, MAX_LEN_NAME) == -1) {
      handle_error("read()");
    }
    clients[i].cid = i;
    clients[i].sock = sock;
    clients[i].addr = cl_addr;
    memset(src, 0, sizeof(src));
    inet_ntop(AF_INET, (struct sockaddr *)&cl_addr.sin_addr, src, sizeof(src));
    fprintf(stderr, "Client %d is accepted (name=%s, address=%s, port=%d).\n", i, clients[i].name, src, ntohs(cl_addr.sin_port));
  }

  close(rsock);

  int j;
  for (i = 0; i < num_clients; i++) {
    send_data(i, &num_clients, sizeof(int));
    send_data(i, &i, sizeof(int));
	send_data(i, &speaker, sizeof(int)); //発言者番号を送信
    for (j = 0; j < num_clients; j++) {
      send_data(i, &clients[j], sizeof(CLIENT));
    }
  }

  num_socks = max_sock + 1;
  FD_ZERO(&mask);
  FD_SET(0, &mask);

  for (i = 0; i < num_clients; i++) {
    FD_SET(clients[i].sock, &mask);
  }
  fprintf(stderr, "Server setup is done.\n");
}

int control_requests() {
  fd_set read_flag = mask;
  memset(&data, 0, sizeof(CONTAINER));

  fprintf(stderr, "select() is started.\n");

  if (select(num_socks, (fd_set *)&read_flag, NULL, NULL, NULL) == -1) {
    handle_error("select()");
  }

  int i, result = 1;
  for (i = 0; i < num_clients; i++) {
    if (FD_ISSET(clients[i].sock, &read_flag)) {
      receive_data(i, &data, sizeof(data));
      switch (data.command) {
      case MESSAGE_COMMAND: //クライアントが'M'と送ってきた時
		/*'M'と送信してきたクライアント番号が，発言者番号と等しい場合，
		  つまり，発言順が正しい場合*/
	    if(speaker == data.cid){
          fprintf(stderr, "client[%d] %s: message = %s\n", clients[i].cid, clients[i].name, data.message);
          speaker++; //発言権を次のクライアントに進める
          if(speaker == num_clients){
            speaker = 0;
          }
		  data.speaker = speaker; //構造体の発言者番号を示す変数に代入
		  send_data(BROADCAST, &data, sizeof(data));
          result = 1;
          break;
	    }
		//発言権が無いクライアントが発言した場合
	    else if(speaker != data.cid){
		  data.command = 'W'; //コマンドに警告を表すWを入力
		  strcpy(data.warning_message, warning_temp); //警告文を代入
		  data.speaker = speaker; //構造体の発言者番号を示す変数に代入
		  send_data(data.cid, &data, sizeof(data)); //警告文を含めて送信
		  result = 1;
		  break;
		}
		
      case QUIT_COMMAND:
        fprintf(stderr, "client[%d] %s: quit\n", clients[i].cid, clients[i].name);
		send_data(BROADCAST, &data, sizeof(data));
        result = 0;
        break;
      default:
        fprintf(stderr, "control_requests(): %c is not a valid command.\n", data.command);
        exit(1);
      }
    }
  }

  return result;
}

static void send_data(int cid, void *data, int size) {
  if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) {
    fprintf(stderr, "send_data(): client id is illeagal.\n");
    exit(1);
  }
  if ((data == NULL) || (size <= 0)) {
    fprintf(stderr, "send_data(): data is illeagal.\n");
  	exit(1);
  }
  if (cid == BROADCAST) {
    int i;
    for (i = 0; i < num_clients; i++) {
      if (write(clients[i].sock, data, size) < 0) {
        handle_error("write()");
      }
    }
  } else {
    if (write(clients[cid].sock, data, size) < 0) {
      handle_error("write()");
    }
  }
}

static int receive_data(int cid, void *data, int size) {
  if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) {
    fprintf(stderr, "receive_data(): client id is illeagal.\n");
    exit(1);
  }
  if ((data == NULL) || (size <= 0)) {
    fprintf(stderr, "receive_data(): data is illeagal.\n");
  	exit(1);
  }

  return read(clients[cid].sock, data, size);
}

static void handle_error(char *message) {
  perror(message);
  fprintf(stderr, "%d\n", errno);
  exit(1);
}

void terminate_server(void) {
  int i;
  for (i = 0; i < num_clients; i++) {
    close(clients[i].sock);
  }
  fprintf(stderr, "All connections are closed.\n");
  exit(0);
}
