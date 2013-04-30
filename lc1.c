#include <errno.h>
#include <stdio.h>
#include "countlines.h"

void main (int argc, char** argv){
   // For all files...
   int i, fd, curlines;
   int totlines = 0;
   for (i = 1; i < argc; i ++){
      // Open the file.
      fd = openFile(argv[i]);
      if (fd < 0){
         fprintf(stderr, "Could not open file: '%s'.\n", argv[i]);
         perror(argv[0]);
         continue;
      }

      // Count the lines.
      curlines = countLines(fd);
      if (curlines < 0){
         printf("     ERR   %s   ERR: Could not count.\n", argv[i]);
      }
      else{
         totlines += curlines;
         printf("%8d   %s\n", curlines, argv[i]);
      }

      // Close the file.
      closeFile(fd);
   }

   // Print total:
   printf("\033[32m%8d   total\033[0m\n", totlines);
}
