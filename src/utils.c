#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <regex.h>
// ONLY THE URL: [a-zA-Z0-9\\.-]+\\.[A-Za-z]+
#define REGEX_HOSTNAME "Host:\\s+([a-zA-Z0-9\\.-]+\\.[A-Za-z]+)(:|\r|\n)"
#define REGEX_PORT_IN_HOSTNAME "Host:\\s+[a-zA-Z0-9\\.-]+\\.[A-Za-z]+:([0-9]+)(|\r|\n)"
#define REGEX_METHOD "^GET"


char* get_hostname_regex(char *http_request, char* hostname) {
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
    int len = m[1].rm_eo - m[1].rm_so;
    strncpy(hostname, (http_request + m[1].rm_so), len);
    regfree(&regex_hostname);
    return hostname;
  }

  regfree(&regex_hostname);
  return NULL;
}

char* get_hostname(char *http_request, char* hostname) {
  char t1[300],t2[300],t3[10];
  char* temp;
  sscanf(http_request,"%s %s %s",t1,t2,t3);
  strcpy(t1,t2);
  int flag=0;
  for(int i=7; i<strlen(t2); i++)
  {
          if(t2[i]==':')
          {
                  flag=1;
                  break;
          }
  }

  temp=strtok(t2,"//");
  if(flag==0)
  {
          temp=strtok(NULL,"/");
  }
  else
  {
          temp=strtok(NULL,":");
  }

  sprintf(t2,"%s",temp);
  strncpy(hostname, t2, 300);
  return hostname;
}


char* get_port(char *http_request) {
  regex_t regex_port;
  int status = regcomp(&regex_port, REGEX_PORT_IN_HOSTNAME, REG_EXTENDED);
  if(status  != 0) {
    perror("Error when compiling the regex\n");
    exit(1);
  }

  char * p = http_request;        // "P" is a pointer into the string which points to the end of the previous match.
  const int n_matches = 2;        // "N_matches" is the maximum number of matches allowed.
  regmatch_t m[n_matches];        // Contains the match

  if(regexec(&regex_port, p, n_matches, m, 0) == 0) {
    int len = m[1].rm_eo - m[1].rm_so;
    char *port;
    port = (char *) malloc(sizeof(char) * len + 1);
    port[len] = 0;
    strncpy(port, (http_request + m[1].rm_so), len);
    regfree(&regex_port);
    return port;
  }
  regfree(&regex_port);
  return NULL;
}

int is_GET(char *http_request) {
  regex_t regex_method;
  int res;
  int status = regcomp(&regex_method, REGEX_METHOD, REG_EXTENDED);
  if(status  != 0) {
    perror("Error when compiling the regex\n");
    exit(1);
  }
  char * p = http_request;
  res = regexec(&regex_method, p, 0, NULL, 0);
  regfree(&regex_method);
  return res;
}

int is_GET2(char* http_request) {
  return (strncmp(http_request, "GET", 3) != 1);
}


void create_new_http_request(char *old_http_request, char *new_request) {
  char *copy = strdup(old_http_request);
  char *delimiter = "\r\n";
  char *token;
  token = strtok(copy, delimiter);
  while(token != NULL) {
    if(strncmp(token, "GET", 3) == 0) {
      strcat(new_request, token);
      strcat(new_request, "\r\n");
    }
    if(strncmp(token, "Host", 4) == 0) {
      strcat(new_request, token);
      strcat(new_request, "\r\n");
    }
    token = strtok(NULL, delimiter);
  }

  strcat(new_request, "Connection: close\r\n\r\n");
  free(copy);
}
