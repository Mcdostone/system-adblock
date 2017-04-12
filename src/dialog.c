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

  read(d->dialog_socket, buffer, BUFFER_SIZE - 1);
  client *c;
  buffer[BUFFER_SIZE - 1] = 0;
  c = create_client(buffer);
  print_client(c);
  connect_client(c);
  send_http_request(c);
  get_http_response(c);
}

void close_dialog(dialog *d) {
  close(d->dialog_socket);
}

void print_dialog(dialog *d) {
  printf("### MyAdblock dialog socket ###\n");
  printf(" - socket descriptor: %d\n", d->dialog_socket);
}
