#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"
#define LOCALHOST "127.0.0.1"


server* create_server(int port) {
  server* s;
  s = (server *) malloc(sizeof(server));
  memset(&(s->serv_addr), 0, sizeof(s->serv_addr));
  s->serv_addr.sin_family = AF_INET;
  s->serv_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
  s->serv_addr.sin_port = port == 0 ? 0 : htons(port);
  s->server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(s->server_socket < 0) {
    perror("Error on server: cannot create the server socket\n");
    exit(1);
  }
  return s;
}

void bind_server(server *s) {
  if(s->server_socket >= 0) {
    if(bind(s->server_socket, (struct sockaddr *) &(s->serv_addr), sizeof(s->serv_addr)) < 0) {
      perror("Error on server: cannot bind the server with the given IP address or port\n");
      exit (1);
    }
    // Need to call getsockname in order to have the port the socket is currently listening
    socklen_t len = sizeof(s->server_socket);
    if (getsockname(s->server_socket, (struct sockaddr *) &(s->serv_addr), &len) == -1) {
      perror("getsockname");
      exit(1);
    }
  }
}

void listen_server(server *s) {
  if(s->server_socket >= 0) {
    if(listen(s->server_socket, SOMAXCONN) < 0) {
      perror("Error on server: cannot listen\n");
      exit (1);
    }
  }
}

dialog* accept_server(server *s) {
  if(s->server_socket >= 0) {
    int numDialog;
    dialog* d = create_dialog();
    numDialog = accept(s->server_socket, (struct sockaddr *) &(d->cli_addr), (socklen_t *)&(d->clilen));
    d->dialog_socket = numDialog;
    return d;
  }
  return NULL;
}

void print_server(server *s) {
  printf("### MyAdblock server ###\n");
  printf(" - Socket descriptor: %d\n", s->server_socket);
  printf(" - IP address: %s\n", get_server_address(s));
  printf(" - Port: %d\n\n", get_server_port(s));
}

int get_server_port(server *s) {
  return ntohs(s->serv_addr.sin_port);
}

char* get_server_address(server *s) {
  return inet_ntoa(s->serv_addr.sin_addr);
}
