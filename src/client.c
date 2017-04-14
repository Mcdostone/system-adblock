#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include "utils.h"
#include "client.h"
#define DEFAULT_PORT "80"
#define LIMIT_CHARS_HTTP_REQUEST 180
#define BUFFER_SIZE 200000


client* create_client(char *http_request) {
  client* c;
  c = NULL;
  if(is_GET(http_request) == 0) {
    c = (client *) malloc(sizeof(client));
    memset(&(c->hints), 0, sizeof(c->hints));
    memset(c->new_http_request, 0, HTTP_REQUEST_SIZE);
    c->hints.ai_family = AF_INET;
    c->http_request = http_request;
    c->hints.ai_socktype = SOCK_STREAM;

    c->hostname = get_hostname(c->http_request);
    getaddrinfo(c->hostname, DEFAULT_PORT, &(c->hints), &(c->res));
    c->client_socket = socket(c->res->ai_family, c->res->ai_socktype, c->res->ai_protocol);

    if(c->client_socket < 0) {
      perror("Error on client socket: cannot create the client socket\n");
      exit(1);
    }
  }
  return c;
}

void connect_client(client *c) {
  connect(c->client_socket, c->res->ai_addr, c->res->ai_addrlen);
  if(DEBUG == 1) {
      struct sockaddr_in* saddr = (struct sockaddr_in*) c->res->ai_addr;
      printf("-- Connected to %s - %s\n",c->hostname, inet_ntoa(saddr->sin_addr));
  }
}

void send_http_request(client *c) {
  create_new_http_request(c->http_request, c->new_http_request);
  if(DEBUG == 1)
    printf("-- Sending request to '%s'\n%s\n", c->hostname, c->new_http_request);
  send(c->client_socket, c->new_http_request, strlen(c->new_http_request), 0);
}

void get_http_response(client *c, dialog *d) {
  char buffer[BUFFER_SIZE];
  int count = 1;

  if(DEBUG == 1)
    printf("-- Send response to the browser from '%s'\n", c->hostname);

  do {
    memset(buffer, 0, BUFFER_SIZE);
    count = recv(c->client_socket, buffer, BUFFER_SIZE, 0);
    if(count > 0)
      send(d->dialog_socket,buffer, count, 0);
  } while(count > 0);
}

void handle_request(client *c, dialog *d) {
  connect_client(c);
  send_http_request(c);
  get_http_response(c, d);
}

void close_client(client *c) {
  close(c->client_socket);
  freeaddrinfo(c->res);
}

void print_client(client *c) {
  char *port = get_port(c->http_request);
  printf("\n-- MyAdblock client socket\n");
  printf("\t- socket descriptor: %d\n", c->client_socket);
  printf("\t- Hostname: %s\n", c->hostname);
  printf("\t- Port: %s\n", (port == NULL ? DEFAULT_PORT : port));
  // if (HTTP request is too big with cookies, others keys ...), we don't print all info
  if(DEBUG == 1)
    printf("\t- new HTTP request:\n%s\n\n", c->new_http_request);
  else {
    if(strlen(c->new_http_request) > LIMIT_CHARS_HTTP_REQUEST)
      printf("\t- new HTTP request (%d chars):\n%.*s...\n\n", LIMIT_CHARS_HTTP_REQUEST, LIMIT_CHARS_HTTP_REQUEST, c->new_http_request);
    else
      printf("\t- new HTTP request:\n%s\n\n", c->new_http_request);
  }

  if(port != NULL)
    free(port);
}
