#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include "dialog.h"
#define HTTP_REQUEST_SIZE 1024

/**
 * Structure containing all structures about the client for MyAdblock
 *
 *	@author Quentin Tardivon
 *	@author Yann Prono
 */

typedef struct client {
  int client_socket;
  char* hostname;
  char* http_request;
  struct addrinfo hints, *res;
  char new_http_request[HTTP_REQUEST_SIZE];
} client;

/**
  Create the client, setup structure for contacting the web server depending of the given HTTP request.
 */
client* create_client(char *http_request);

void handle_request(client *c, dialog *d);

void connect_client(client *c);

void send_http_request(client *c);

void get_http_response(client *c, dialog *d);

void close_client(client *c);

void print_client(client *c);

#endif
