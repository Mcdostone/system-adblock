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
  char *hostname;

  if(is_GET(http_request) == 0) {
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
  }
  return c;
}

void connect_client(client *c) {
  connect(c->client_socket, c->res->ai_addr, c->res->ai_addrlen);
}

void send_http_request(client *c) {
  char *hostname = get_hostname(c->http_request);
  if(DEBUG == 1)
    printf("-- [%d] Sending request to '%s'\n", c->client_socket, hostname);
  send(c->client_socket, c->http_request, strlen(c->http_request), 0);
  free(hostname);
}

void get_http_response(client *c, dialog *d) {
  char *hostname = get_hostname(c->http_request);
  char buffer[BUFFER_SIZE];
  int count = 1;

  if(DEBUG == 1)
    printf("-- [%d] Send response to the browser from '%s'\n", c->client_socket, hostname);

  /*do {
    count = read(c->client_socket, buffer ,BUFFER_SIZE - 1);
    printf("%s\n", buffer);
    send(d->dialog_socket, buffer, count, MSG_DONTWAIT);
    count = count < BUFFER_SIZE ? -1 : count;
  } while (count != -1);*/

  while((count = read(c->client_socket, buffer ,BUFFER_SIZE - 1)) != 0) {
    send(d->dialog_socket, buffer, count, MSG_DONTWAIT);
    printf("%s\n %d\n", buffer, count);
  }

  if(DEBUG == 1)
    printf("-- [%d] All data received by web browser [%d]\n", c->client_socket, d->dialog_socket);

  free(hostname);
}

void handle_request(client *c, dialog *d) {
  if(DEBUG == 1)
    print_client(c);
  connect_client(c);
  send_http_request(c);
  get_http_response(c, d);
}

void close_client(client *c) {
  if(DEBUG == 1)
    printf("-- [%d] Close client socket\n", c->client_socket);
  close(c->client_socket);
}

void print_client(client *c) {
  char *hostname = get_hostname(c->http_request);
  char *port = get_port(c->http_request);
  printf("\n-- MyAdblock client socket\n");
  printf("\t- socket descriptor: %d\n", c->client_socket);
  printf("\t- Hostname: %s\n", hostname);
  printf("\t- Port: %s\n", (port == NULL ? DEFAULT_PORT : port));
  // if (HTTP request is too big with cookies, others keys ...), we don't print all info
  if(DEBUG == 1)
    printf("\t- HTTP request:\n%s\n\n", c->http_request);
  else {
    if(strlen(c->http_request) > LIMIT_CHARS_HTTP_REQUEST)
      printf("\t- HTTP request (%d chars):\n%.*s...\n\n", LIMIT_CHARS_HTTP_REQUEST, LIMIT_CHARS_HTTP_REQUEST, c->http_request);
    else
      printf("\t- HTTP request:\n%s\n\n", c->http_request);
  }

  free(hostname);
  if(port != NULL)
    free(port);
}
