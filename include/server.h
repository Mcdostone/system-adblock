#ifndef SERVER_H
#define SERVER_H

#include "dialog.h"

/**
 * Structure containing all information about the server for MyAdblock
 *
 *	@author Quentin Tardivon
 *	@author Yann Prono
 */

typedef struct server {
  int server_socket;
  struct sockaddr_in serv_addr;
} server;

/**
  Allocate memory for the server structure and create the socket.
  By default, the IP address of the server is 127.0.0.1.
 */
server* create_server(int port);

/**
  Bind the server.
*/
void bind_server(server *s);

/**
  listen for connections on the socket server.
*/
void listen_server(server *s);

/**
  wait and accept a connection on the server socket.
*/
void accept_server(server *s);

/**
  Close the server socket.
  */
void close_server(server *s);

/**
  Print information about the given server structure.
 */
void print_server(server *s);

/**
  Returns the port the socket server is listening on.
*/
int get_server_port(server *s);

/**
  Returns the IP address on the server.
*/
char* get_server_address(server *s);

#endif
