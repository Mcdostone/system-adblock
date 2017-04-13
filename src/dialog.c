#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include "dialog.h"
#include "client.h"
#define BUFFER_SIZE 4096


dialog* create_dialog() {
  dialog* d = (dialog *) malloc(sizeof(dialog));
  d->clilen = sizeof(d->cli_addr);
  return d;
}

void read_request(dialog *d) {
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(char) * BUFFER_SIZE);

  // BUG Strange, impossible to exit this loop, I don't know why
  // The most strange is there is only 1 print in the terminal ...
  /*int tmp;
  while((tmp = read(d->dialog_socket, buffer, BUFFER_SIZE - 1)) > 0) {
    printf("%s %d\n", buffer, tmp);
  }*/

  recv(d->dialog_socket, buffer, BUFFER_SIZE - 1, 0);
  client *c;
  buffer[BUFFER_SIZE - 1] = 0;
  c = create_client(buffer);
  if( c != NULL) {
    if(DEBUG == 1)
      print_dialog(d);
    handle_request(c, d);
    close_client(c);
  }
  else
    printf("-- Ignore following request: %.*s...\n\n", 20, buffer);
}

void close_dialog(dialog *d) {
  close(d->dialog_socket);
}

void print_dialog(dialog *d) {
  printf("\n-- MyAdblock dialog socket\n");
  printf("\t- socket descriptor: %d\n\n", d->dialog_socket);
}
