/***************************************************************************************************************
   SOURCES

 * Title    : How to use the typedef struct in C
 * Author   : Educative.io
 * Date     : n.d
 * Accessed : 2025-05-03
 * URL      : https://www.educative.io/answers/how-to-use-the-typedef-struct-in-c

 * Title    : Passing Arguments to Threads
 * Author   : hpc-tutorials.llnl.gov (Lawrence Livermore National Laboratory)
 * Date     : n.d
 * Accessed : 2025-05-03
 * URL      : https://hpc-tutorials.llnl.gov/posix/passing_args/

 * Title    : Mutex lock Linux Thread Synchronization
 * Author   : GeeksForGeeks.org
 * Date     : 2024-05-24
 * Accessed : 2025-05-03
 * URL      : https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

 * Title    : pthread_mutex_init
 * Author   : pubs.opengroup.org
 * Date     : n.d
 * Accessed : 2025-05-03
 * URL      : https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_init.html

 * Title    : How to use POSIX sempaphores in C language
 * Author   : GeeksForGeeks.org
 * Date     : 2025-01-10
 * Accessed : 2025-05-03
 * URL      : https://www.geeksforgeeks.org/use-posix-semaphores-c/

***************************************************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "dbj2.h"
#include "tea.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Struct to remove global variables and to contain more data that might come
typedef struct THREADDATA {
   int count[BYTE_RANGE];
   unsigned char buffer[BUFFER_SIZE];
   pthread_mutex_t mutex;
   int bytes_in_buffer;
   char psFilePath[BUFFER_SIZE];
   // Two sempahores to replace previous conditions, one for full and one for empty.
   sem_t sem_full, sem_empty;
} tThreadData;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* thread_A(void* arg) {
   tThreadData *pThreadData = (tThreadData *) arg;

   // Opens the file at the path specified by the struct
   FILE* fp = fopen(pThreadData->psFilePath, "rb");
   if (!fp) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   FILE* fHashFile = fopen("task4_pg2265.hash", "wb");
   if (!fHashFile) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   FILE* fEncryptionFile = fopen("task4_pg2265.enc", "wb");
   if (!fEncryptionFile) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   while (1) {
      pthread_mutex_lock(&pThreadData->mutex);
      while (pThreadData->bytes_in_buffer == BUFFER_SIZE) {
         // Unlock the mutex, wait for the sempahore to answer "empty", and then lock the mutex again.
         pthread_mutex_unlock(&pThreadData->mutex);
         sem_wait(&pThreadData->sem_empty);
         pthread_mutex_lock(&pThreadData->mutex);
      }

      int read_bytes = fread(pThreadData->buffer + pThreadData->bytes_in_buffer, 1, BUFFER_SIZE - pThreadData->bytes_in_buffer, fp);
      pThreadData->bytes_in_buffer += read_bytes;

      if (read_bytes < BUFFER_SIZE - pThreadData->bytes_in_buffer) {
         pthread_mutex_unlock(&pThreadData->mutex);
         break;
      }
 
      // Unlock the mutex, and post that the buffer is now full.
      pthread_mutex_unlock(&pThreadData->mutex);
      sem_post(&pThreadData->sem_full);

      
      // Code for part 2: Calculate a DBJ2 hash 
      unsigned int *piHash = malloc(sizeof(unsigned int));

      Task2_SimpleDjb2Hash(fp, piHash);

      fwrite(&piHash, sizeof(piHash), 1, fHashFile);

      printf("A DJB2 hash: %x\n", &piHash);

      free(piHash);
      fclose(fHashFile);
      
      // Code for part 2: Create a TEA encrypted version
      // Code is not complete 
      /*
      unsigned int *v = malloc(sizeof(unsigned int));
      unsigned int *w = malloc(sizeof(unsigned int));
      unsigned int *k = malloc(sizeof(unsigned int));

      encipher(v, w, k);
      
      fwrite(&v, sizeof(fEncryptionFile), 1, fEncryptionFile);
      fwrite(&w, sizeof(fEncryptionFile), 1, fEncryptionFile);
      fwrite(&k, sizeof(fEncryptionFile), 1, fEncryptionFile);

      printf("A TEA encryption: %s, %s, %s\n", &v, &w, &k);
      */
      
   }
   fclose(fp);
   
   pthread_exit(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* thread_B(void* arg) {

   tThreadData *pThreadData = (tThreadData *) arg;

   memset(pThreadData->count, 0, sizeof(pThreadData->count));

   while (1) {
      pthread_mutex_lock(&pThreadData->mutex);
      while (pThreadData->bytes_in_buffer == 0) {
         // Unlock the mutex, wait for the sempahore to answer "full", and then lock the mutex again.
         pthread_mutex_unlock(&pThreadData->mutex);
         sem_wait(&pThreadData->sem_full);
         pthread_mutex_lock(&pThreadData->mutex);
      }

      // Commented out as a requirement for part 2 of the task.
      //for (int i = 0; i < pThreadData->bytes_in_buffer; i++)
           //pThreadData->count[pThreadData->buffer[i]]++;

      pThreadData->bytes_in_buffer = 0;
      
      // Unlock the mutex and post to the empty semaphore.
      pthread_mutex_unlock(&pThreadData->mutex);
      sem_post(&pThreadData->sem_empty);

      if (pThreadData->bytes_in_buffer == 0)
         break;
   }
   for (int i = 0; i < BYTE_RANGE; i++)
      printf("%d: %d\n", i, pThreadData->count[i]);
   pthread_exit(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int iArgc, char *apszArgv[]) {

   pthread_t threadA, threadB;

   // Allocate space for the struct
   tThreadData *tData = malloc(sizeof(tThreadData));
   // Through a command line argument we recieve the wanted file
   strcpy(tData->psFilePath, apszArgv[1]);
   tData->bytes_in_buffer = 0;

   // Initialize semaphores
   sem_init(&tData->sem_full, 0, 0);
   sem_init(&tData->sem_empty, 0, 0);

   // Attempt to initialize the mutex within the structure tData
   if (pthread_mutex_init(&tData->mutex, NULL) != 0) {
      perror("Could not create mutex init");
      exit(1);
   }
   
   if (pthread_create(&threadA, NULL, thread_A, (void*)tData) != 0) {
      perror("Could not create thread A");
      free(tData);
      exit(1);
   }

   if (pthread_create(&threadB, NULL, thread_B, (void*)tData) != 0) {
      perror("Could not create thread B");
      free(tData);
      exit(1);
   }

   if (pthread_join(threadA, NULL) != 0) {
      perror("Could not join thread A");
      exit(1);
   }

   if (pthread_join(threadB, NULL) != 0) {
      perror("Could not join thread B");
      exit(1);
   }
  
   // Destroy mutex when no longer needed to avoid resource leaks.
   pthread_mutex_destroy(&tData->mutex);

   free(tData);

   return 0;
}
