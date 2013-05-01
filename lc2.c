#include <errno.h>
#include <stdio.h>
#include <unistd.h>

void forkChild(char*);

int numchildren = 0;

void main (int argc, char** argv){
   // Set up pipe
   int pipefd[2];
   if (pipe(pipefd) != 0){
      perror("Couldn't create pipe.");
      exit(1);
   }
   int readfd = pipefd[0];
   int writefd = pipefd[1];
   int shout = dup(1);

   // Set up stdout for children.
   dup2(stdout, writefd);

   // Create children.
   int i;
   for (i = 1; i < argc; i++)
      forkChild(argv[i]);

   // Wait for children.
   int status;
   while(numchildren > 0){
      waitpid(-1, &status, 0);
      // TODO Handle status.
   }

   // Write results.
   char buf[100];
   int chars;
   while ((chars = read(readfd, &buf, 100)) > 0)
      write(shout, &buf, chars);
}

void forkChild(char* file){
   int pid = fork();

   // Fork failed:
   if (pid < 0){
      perror("Could not fork.");
      exit(1);
   }

   // Parent:
   if (pid){
      numchildren++;
   }

   // Child:
   else{
      execl("lc1", "lc2", file, NULL);
      perror("Could not exec.");
      exit(1);
   }
}
