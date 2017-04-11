#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <regex.h>
#define REGEX_HOSTNAME "Host:\\s+([a-zA-Z0-9\\.-]+)(\r|\n)"


char* get_ip_address(char *host) {
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h;
  char *ip;
  ip = (char *) malloc(sizeof(char) * 100);
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if(getaddrinfo(host , "http" , &hints , &servinfo) != 0)
    fprintf(stderr, "error");

  for(p = servinfo; p != NULL; p = p->ai_next) {
    h = (struct sockaddr_in *) p->ai_addr;
    strcpy(ip , inet_ntoa(h->sin_addr));
    printf("Maybe IP is %s\n", ip);
  }
  return NULL;
}

char* extract_hostname(char *http_request) {
  regex_t regex_hostname;
  int status = regcomp(&regex_hostname, REGEX_HOSTNAME, REG_EXTENDED);
  if(status  != 0) {
    perror("Error when compiling the regex\n");
    exit(1);
  }

  char * p = http_request;        // "P" is a pointer into the string which points to the end of the previous match.
  const int n_matches = 2;        // "N_matches" is the maximum number of matches allowed.
  regmatch_t m[n_matches];        // Contains the match

  if(regexec(&regex_hostname, p, n_matches, m, 0) == 0) {
    if(m[1].rm_so == (size_t) - 1)
      return NULL;
    int len = m[1].rm_eo - m[1].rm_so;
    char *hostname;
    hostname = (char *) malloc(sizeof(char) * len + 1);
    hostname[len] = 0;
    strncpy(hostname, (http_request + m[1].rm_so), len);
    return hostname;
  }
  return NULL;
}
