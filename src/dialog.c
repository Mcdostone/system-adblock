#include <stdlib.h>
#include <stdio.h>
#include "dialog.h"


dialog* create_dialog() {
  dialog* d = (dialog *) malloc(sizeof(dialog));
  d->clilen = sizeof(d->cli_addr);
  return d;
}

void print_dialog(dialog *d) {
  printf("### MyAdblock dialog socket ###\n");
  printf(" - socket descriptor: %d\n", d->dialog_socket);
}
