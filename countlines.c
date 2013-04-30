#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "countlines.h"

#define MAX_BUF 1024

/* openFile
 * Opens the given file as read only.
 * If successful, returns the file descriptor.
 * Otherwise, returns -1.
 * On error, errno should be set appropriately. */
int openFile (const char* path){
   int fd = open(path, O_RDONLY);
   return fd < 0 ? -1 : fd;
}

/* closeFile
 * Closes the given file.
 * No error checking is performed, as no remedial action can
 * be taken on a file-close error. */
int closeFile(int fd){
   close(fd);
}

/* countLines
 * Takes a file descriptor and counts the number of new line
 * characters it contains.
 * On error, returns -1; */
int countLines (int fd){
   char* buf = malloc(MAX_BUF);
   if (buf == NULL) return -1;

   // Load initial buffer.
   int index = 0, lines = 0;
   int bufsize = read(fd, buf, MAX_BUF);
   if (bufsize < 0) return -1;

   while (index < bufsize){
      // Check if new line and advance.
      if (buf[index++] == '\n') lines++;

      // Refill buffer, if needed.
      if (index == bufsize){
         bufsize = read(fd, buf, MAX_BUF);
         if (bufsize < 0) return -1;
         index = 0;
      }
   }

   return lines;
}
