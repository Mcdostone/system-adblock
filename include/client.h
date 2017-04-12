#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>

/**
 * Structure containing all structures about the client for MyAdblock
 *
 *	@author Quentin Tardivon
 *	@author Yann Prono
 */

typedef struct client {
  int client_socket;
  char* http_request;
  struct addrinfo hints, *res;
} client;

/**
  Create the client, setup structure for contacting the web server depending of the given HTTP request.
 */
client* create_client(char *http_request);

void connect_client(client *c);

void send_http_request(client *c);

void get_http_response(client *c);

void print_client(client *c);

#endif
