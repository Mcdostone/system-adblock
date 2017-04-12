#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "utils.h"
#include "client.h"
#define DEFAULT_PORT "80"
#define LIMIT_CHARS_HTTP_REQUEST 180
#define BUFFER_SIZE 40000


client* create_client(char *http_request) {
  client* c;
  char *hostname;

  c = (client *) malloc(sizeof(client));

  memset(&(c->hints), 0, sizeof(c->hints));
  c->hints.ai_family = AF_UNSPEC;
  c->http_request = http_request;
  c->hints.ai_socktype = SOCK_STREAM;

  hostname = get_hostname(c->http_request);
  getaddrinfo(hostname, DEFAULT_PORT, &(c->hints), &(c->res));
  c->client_socket = socket(c->res->ai_family, c->res->ai_socktype, c->res->ai_protocol);

  if(c->client_socket < 0) {
    perror("Error on client socket: cannot create the client socket\n");
    exit(1);
  }

//  freeaddrinfo(c->res);
  free(hostname);
  return c;
}

void connect_client(client *c) {
  // blocking instruction
  connect(c->client_socket, c->res->ai_addr, c->res->ai_addrlen);
  if(DEBUG == 1)
    printf("-- [%d] Connected!\n", c->client_socket);
}

void send_http_request(client *c) {
  if(DEBUG == 1)
    printf("-- [%d] Sending request\n", c->client_socket);
  send(c->client_socket, c->http_request, strlen(c->http_request), 0);
}

void get_http_response(client *c) {
  char buffer[BUFFER_SIZE];
  int count;

  if(DEBUG == 1)
    printf("-- [%d] getting response\n", c->client_socket);
  while((count = recv(c->client_socket, buffer ,BUFFER_SIZE - 1, 0)) != 0) {
    buffer[count] = 0;
    printf("-- [%d] received: \n%s\n", c->client_socket, buffer);
  }

  if(DEBUG == 1)
    printf("-- [%d] Data received\n", c->client_socket);
}

void print_client(client *c) {
  char *hostname = get_hostname(c->http_request);
  char *port = get_port(c->http_request);
  printf("### MyAdblock client socket ###\n");
  printf(" - socket descriptor: %d\n", c->client_socket);
  printf(" - Hostname: %s\n", hostname);
  printf(" - Port: %s\n", (port == NULL ? DEFAULT_PORT : port));
  // if (HTTP request is too big with cookies, others keys ...), we don't print all info
  if(strlen(c->http_request) > LIMIT_CHARS_HTTP_REQUEST)
    printf(" - HTTP request (%d chars):\n%.*s...\n\n", LIMIT_CHARS_HTTP_REQUEST, LIMIT_CHARS_HTTP_REQUEST, c->http_request);
  else
    printf(" - HTTP request:\n%s\n\n", c->http_request);

  free(hostname);
  if(port != NULL)
    free(port);
}
