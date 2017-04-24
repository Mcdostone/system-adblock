#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include "dialog.h"
#include "client.h"
#include "adfilter.h"


dialog* create_dialog() {
  dialog* d = (dialog *) malloc(sizeof(dialog));
  d->clilen = sizeof(d->cli_addr);
  return d;
}

void read_request(dialog *d) {
  char buffer[HTTP_REQUEST_SIZE];
  memset(buffer, 0, sizeof(char) * HTTP_REQUEST_SIZE);
  recv(d->dialog_socket, buffer, HTTP_REQUEST_SIZE - 1, 0);
  client *c;
  buffer[HTTP_REQUEST_SIZE - 1] = 0;
  if(DEBUG == 1)
    printf("%s\n",buffer);

  char* buffercpy = strdup(buffer);
  if (isAd(buffercpy) == 0) {
    char *request_client = strdup(buffer);
    c = create_client(request_client);
    if( c != NULL) {
      handle_request(c, d);
      close_client(c);
      free(c);
      free(request_client);
    }
    else {
      if(DEBUG == 1)
        printf("-- Ignore following request: %s...\n\n", buffer);
      free(request_client);
    }
  }
  else {
    //printf("## Reject request: %.*s...\n", 50, buffer);
    printf("## Reject request: %s...\n", buffer);
    close_dialog(d);
  }
  close_dialog(d);
  free(buffercpy);
}

void close_dialog(dialog *d) {
  close(d->dialog_socket);
}

void print_dialog(dialog *d) {
  printf("\n-- MyAdblock dialog socket\n");
  printf("\t- socket descriptor: %d\n\n", d->dialog_socket);
}
