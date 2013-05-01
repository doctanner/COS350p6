#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printResults();
void forkChild(char*);

int numchildren = 0;
int numjobs;

char** jobs;
int* infds;

void main (int argc, char** argv){
   // Set up arrays.
   numjobs = argc - 1;
   jobs = malloc(sizeof(char*) * numjobs);
   infds = malloc(sizeof(int) * numjobs);

   // Test mallocs.
   if (jobs == NULL || infds == NULL){
      fprintf(stderr, "Malloc error.");
      exit(1);
   }

   // Set up stdout.
   int shout = dup(1);

   // Create children.
   int pipefd[2];
   int i, writefd;
   for (i = 1; i < argc; i++){
      // Set up pipe.
      if (pipe(pipefd) != 0){
         perror("Couldn't create pipe.");
         exit(1);
      }

      // Save to arrays for output.
      infds[i - 1] = pipefd[0];
      jobs[i - 1] = argv[i];

      // Replace stdout for child.
      dup2(pipefd[1], 1);

      // Fork child.
      forkChild(argv[i]);
   }

   // Reset stdout.
   dup2(shout, 1);

   // Wait for children.
   int status;
   while(numchildren > 0){
      waitpid(-1, &status, 0);
      numchildren--;
   }

   // Print results.
   printResults();
}

/* printResults
 * Print the results that have been found. */
void printResults(){
   // Write results.
   int val,i;
   int totlines = 0;
   for (i = 0; i < numjobs; i++){
      read(infds[i], &val, sizeof(int));
      if (val < 0){
         fprintf(stdout,"     ERR   %s   ERR: Could not count.\n", jobs[i]);
      }
      else{
         fprintf(stdout, "%8d   %s\n",val, jobs[i]);
         totlines+=val;
      }
   }

   // Write total.
   if (numjobs > 1){
      fprintf(stdout, "\033[32m%8d   total\033[0m\n",totlines);
   }
}

/* forkChild
 * Create a child process to do the work. */
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
