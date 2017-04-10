#ifndef DIALOG_H
#define DIALOG_H

#include <arpa/inet.h>

/**
 * Structure containing all information about the dialog socket for MyAdblock
 *
 *	@author Quentin Tardivon
 *	@author Yann Prono
 */

typedef struct dialog {
  int dialog_socket;
  int clilen;
  struct sockaddr_in cli_addr;
} dialog;

/**
  Allocate memory for the dialog socket structure and create the socket.
 */
dialog* create_dialog();

void print_dialog(dialog *d);

#endif
