#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include "dialog.h"
#include "utils.h"
#define BUFFER_SIZE 1024
#define URL_REGEX   "GET (.*) HTTP\\/1.1"


dialog* create_dialog() {
  dialog* d = (dialog *) malloc(sizeof(dialog));
  d->clilen = sizeof(d->cli_addr);
  return d;
}

void read_request(dialog *d) {
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
  regex_t re;

  // Build regex to extract URL from HTTP request
  if(regcomp(&re, URL_REGEX, REG_EXTENDED) != 0) {
    fprintf(stderr, "Failed to compile regex '%s'\n", URL_REGEX);
    exit(0);
  }

  // BUG Strange, impossible to exit this loop, I don't know why
  // The most strange is there is only 1 print in the terminal ...
  /*int tmp;
  while((tmp = read(d->dialog_socket, buffer, BUFFER_SIZE - 1)) > 0) {
    printf("%s %d\n", buffer, tmp);
  }*/

  read(d->dialog_socket, buffer, BUFFER_SIZE - 1);
  // Prints only X first chars
  printf ("%.*s...\n", 180, buffer);
  char *hostname = extract_hostname(buffer);
  printf("\n## The host is '%s'\n\n", hostname);
}

void close_dialog(dialog *d) {
  close(d->dialog_socket);
}


void print_dialog(dialog *d) {
  printf("### MyAdblock dialog socket ###\n");
  printf(" - socket descriptor: %d\n", d->dialog_socket);
}
