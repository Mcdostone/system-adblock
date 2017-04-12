#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include "server.h"
#include "utils.h"
#define BUFFER_SIZE 1024


server *s;

void print_usage(char *pgm) {
  printf("Usage: %s [OPTIONS]\n", pgm);
  printf("Options:\n");
  printf("  %-20s %s\n",      "-p, --port=PORT",  "Specify the port the server socket will listen");
  printf("  %-20s %s\n\n",    "--help",           "Display this help and exit");
}

void  INThandler(int sig) {
     free(s);
     exit(0);
}

int main(int argc, char *argv[]) {
  // parse options
  signal(SIGINT, INThandler);
  int port = 0;
  int opt = 0;
  int long_index = 0;
  struct option long_options[] = {
    {"port",      required_argument,  0,  'p' },
    {"help",      no_argument,        0,  'h' },
    {0,           0,                  0,  0   }
  };
  while((opt = getopt_long(argc, argv, "p:h", long_options, &long_index )) != -1) {
    switch (opt) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'h':
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
      }
  }

  // program
  s = create_server(port);
  bind_server(s);
  listen_server(s);
  printf("Listening on http://%s:%d\n\n", get_server_address(s), get_server_port(s));
  accept_server(s);
}
