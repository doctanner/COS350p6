#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "countlines.h"

void processJob(void *);

char* STR_TOTAL = "total";

struct lc_job
{
   char* file;
   int lines;
};

void main (int argc, char** argv){
   // Set up threads and jobs.
   int numjobs = argc - 1;
   pthread_t *threads;
   threads = malloc(numjobs * sizeof(pthread_t));
   struct lc_job *jobs;
   jobs = malloc(numjobs * sizeof(struct lc_job));
   int i;
   for (i = 0; i < numjobs; i++){
      jobs[i].file = argv[i + 1];
      jobs[i].lines = -2;
      pthread_create(&threads[i], NULL, (void*) &processJob, (void*) &jobs[i]);
   }

   // Wait on all threads.
   for (i = 0; i < numjobs; i++){
      pthread_join(threads[i], NULL);
      if (jobs[i].lines < 0)
         fprintf(stderr, "Could not count '%s'\n", jobs[i].file);
   }

   // Print results.
   int totlines = 0;
   for (i = 0; i < numjobs; i++){
      if(jobs[i].lines >= 0){
         printf("%8d   %s\n", jobs[i].lines, jobs[i].file);
         totlines += jobs[i].lines;
      }
   }

   // Print total:
   printf("\033[32m%8d   total\033[0m\n", totlines);
}

void processJob(void *arg){
   // Get job struct from argument.
   struct lc_job *job = (struct lc_job *) arg;

   // Open the file.
   int fd = openFile(job->file);
   if (fd < 0){
      job->lines = -1;
      pthread_exit(NULL);
   }

   // Count the lines and close the file.
   job->lines = countLines(fd);
   closeFile(fd);
}
