#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "../include/server.h"
#include "../include/utils.h"
#define BUFFER_SIZE 1024

struct sockaddr_in serv_addr;

void print_usage(char *pgm) {
  printf("Usage: %s [OPTIONS]\n", pgm);
  printf("Options:\n");
  printf("  %-20s %s\n",      "-p, --port=PORT",  "Specifie the port the server socket will listen");
  printf("  %-20s %s\n\n",    "--help",           "Display this help and exit");
}

int main(int argc, char *argv[]) {
  // parse options
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
  server *s = create_server(port);
  bind_server(s);
  listen_server(s);
  printf("Listening on http://%s:%d\n\n", get_server_address(s), get_server_port(s));
  accept_server(s);

}
