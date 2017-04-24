#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <utils.h>
#define NB_FILTERS 65000
#define LENGTH_FILTER 500

char* filters[NB_FILTERS];
int loaded = 0;

void load_file() {
  if(DEBUG == 1)
      printf("~~ Load easylist file\n");
  FILE * fp;
  size_t len = 0;
  ssize_t read;
  /*for (int i = 0; i < NB_FILTERS; i++) {
    filters[i] = NULL;
  }*/

  fp = fopen("easylist.txt", "r");
  if (fp == NULL) {
    printf("File not found\n");
    exit(1);
  }

  int i = 0;
  getline(&filters[0], &len, fp);
  while (!feof(fp) && i < NB_FILTERS) {
    read = getline(&filters[i], &len, fp);
    if (filters[i][0] != '!' && filters[i][0] != '|'
    && filters[i][0] != '@' && filters[i][0] != '#') {
      filters[i][read - 1] = 0;
      i = i + 1;
    }
    else {
      filters[i] = NULL;
    }
  }
  filters[i] = NULL;
  loaded = 1;

  if(DEBUG == 1)
      printf("~~ %d filters loaded\n", i);
  fclose(fp);
}

int testRequest(char * address) {
  if(loaded == 0) {
    load_file();
  }
  if(DEBUG == 1)
      printf("~~ Testing the request\n");
  int i = 0;
  while (filters[i] != NULL) {
    if(strstr(address, filters[i]) != NULL && strlen(filters[i])>0) {
      printf("HORRIBLE: %s HFAEIFB %ld\n", filters[i], strlen(filters[i]));
      return 1;
    }
    i = i + 1;
  }

  return 0;

  /*FILE * fp;
   char * line = NULL;
   size_t len = 0;


   fp = fopen("easylist.txt", "r");
   if (fp == NULL) {
     printf("File not found\n");
     return 0;
   }
   printf("Testing the request\n");
   while (feof(fp) == 0) {
      getline(&line, &len, fp);
       if (strcmp(address, line) == 0) {
        printf("Adresse refusée: %s\n Ligne de easylist: %s\n", address, line);
          fclose(fp);

          if (line)
                free(line);
         return 1;
       }
   }

   return 0;
   */
}

int isAd(char* http_request) {
  char hostname[300];
  get_hostname(http_request, hostname);
  return testRequest(hostname);
}



void printDoc() {
  FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen("easylist.txt", "r");
   if (fp == NULL)
       exit(EXIT_FAILURE);

   while ((read = getline(&line, &len, fp)) != -1) {
       printf("Retrieved line of length %zu :\n", read);
       printf("%s", line);
   }

   fclose(fp);

   if (line)
       free(line);
   exit(EXIT_SUCCESS);

}
