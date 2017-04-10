#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "server.h"
#define PORT 3141

struct sockaddr_in serv_addr;

int main(int argc, char *argv[]) {
  server *s = create_server(3411);
  bind_server(s);
  print_server(s);
  printf("Listening on %s:%d\n", get_server_address(s), get_server_port(s));
  listen_server(s);
/*
  int server;

  memset (&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  // Create the socket
  server = socket(PF_INET, SOCK_STREAM, 0);

  if(bind(server,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror ("servecho: erreur bind\n");
    exit (1);
  }

  if(listen(server, SOMAXCONN) < 0) {
    perror ("servecho: erreur listen\n");
    exit (1);
  }

  int dialog;
  int clilen;
  struct sockaddr_in cli_addr;
  clilen = sizeof(cli_addr);
  printf("Listening on %s:%d\n", inet_ntoa(serv_addr.sin_addr), PORT);
  fflush(stdout);
  dialog = accept(server, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

  if (dialog < 0) {
    perror("servecho : erreur accep\n");
    exit (1);
  }

  printf("Hello !\n");
  return 0;*/
}
