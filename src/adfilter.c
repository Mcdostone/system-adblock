#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <utils.h>

int testRequest(char * address) {
  FILE * fp;
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
