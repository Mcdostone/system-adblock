#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#define LOCALHOST "127.0.0.1"


server* create_server(int port) {
  server* s;
  s = (server *) malloc(sizeof(server));

  memset (&(s->serv_addr), 0, sizeof(s->serv_addr));
  s->serv_addr.sin_family = AF_INET;
  s->serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
  s->serv_addr.sin_port = htons(port);

  s->serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  if(s->serverSocket < 0) {
    perror("Error on server: cannot create the server socket\n");
    exit(1);
  }
  return s;
}

void bind_server(server *s) {
  if(s->serverSocket >= 0) {
    if(bind(s->serverSocket, (struct sockaddr *)&(s->serv_addr), sizeof(s->serv_addr)) < 0) {
      perror("Error on server: cannot bind the server with the given IP address or port\n");
      exit (1);
    }
  }
}

void listen_server(server *s) {
  if(s->serverSocket >= 0) {
    if(listen(s->serverSocket, SOMAXCONN) < 0) {
      perror("Error on server: cannot listen\n");
      exit (1);
    }
  }
}

void accept_server(server *s) {
/*  if(s->serverSocket >= 0) {
    numDialog = accept(server, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

    if(accept(s->serverSocket, SOMAXCONN) < 0) {
      perror("Error on server: cannot listen\n");
      exit (1);
    }
  }*/
}


void print_server(server *s) {
  printf("### MyAdblock server ###\n");
  printf(" - IP address: %s\n", get_server_address(s));
  printf(" - Port: %d\n\n", get_server_port(s));
}

int get_server_port(server *s) {
  return ntohs(s->serv_addr.sin_port);
}

char* get_server_address(server *s) {
  return inet_ntoa(s->serv_addr.sin_addr);
}
