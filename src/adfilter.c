#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int testRequest(char * address) {
  FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen("easylist.txt", "r");
   if (fp == NULL) {
     printf("File not found\n");
     return 0;
   }

   while ((read = getline(&line, &len, fp)) != -1) {
       if (strcmp(address, line)) {
         fclose(fp);

          if (line)
                free(line);
         return 1;
       }
   }

   return 0;
}

int isAd(char* http_request) {


   if ((strncmp(http_request, "GET", 3) == 0)) {
      //printf("%s\n", http_request);
      const char s[1] = " ";
      char *token;

      token = strtok(http_request, s);


      token = strtok(NULL, s);
      //printf( "Address: %s\n", token );

      return testRequest(token);
   }

   return 0;
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
