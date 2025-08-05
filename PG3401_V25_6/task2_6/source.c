/***************************************************************************************************************
   SOURCES

 * Title    : The C Programming Language
 * Author   : Kernighan, B. W & Ritchie, D. M
 * Date     : 1988
 * Page     : 207
 * Publisher: Prentice Hall
 * URL      : https://colorcomputerarchive.com/repo/Documents/Books/The%20C%20Programming%20Language%20%28Kernighan%20Ritchie%29.pdf

 * Title    : How to Write a Struct to a Binary File in C?
 * Author   : GeeksForGeeks.org
 * Date     : 2024-03-27
 * Accessed : 2025-04-30
 * URL      : https://www.geeksforgeeks.org/how-to-read-struct-from-binary-file-in-c/

***************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task2_hash.h"
#include "task2_count.h"
#include "task2_sum.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TASK2_FILE_METADATA {
   char szFileName[32];
   int iFileSize;
   char byHash[4];
   int iSumOfChars;
   char aAlphaCount[26];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main() {
   FILE *fOriginalFile;
   fOriginalFile = fopen("pgexam25_test.txt", "r");

   unsigned int *piHash = malloc(sizeof(unsigned int));
   char aCountArray[26] = {0};
   int *piSizeOfFile = malloc(sizeof(int));
   int *piSumOfChars = malloc(sizeof(int));

   Task2_SimpleDjb2Hash(fOriginalFile, piHash);
   Task2_CountEachCharacter(fOriginalFile, aCountArray);
   Task2_SizeAndSumOfCharacters(fOriginalFile, piSizeOfFile, piSumOfChars);

   FILE *fOutputFile;
   fOutputFile = fopen("pgexam25_output.bin", "wb");

   struct TASK2_FILE_METADATA fileMetadata = {"pgexam25_test.txt", *piSizeOfFile, "", *piSumOfChars, *aCountArray};

   memcpy(fileMetadata.byHash, piHash, 4);

   fwrite(&fileMetadata, sizeof(struct TASK2_FILE_METADATA), 1, fOutputFile);

   printf("A DJB2 hash: %x\n", fileMetadata.byHash);
   printf("The count of each letter: %i\n", aCountArray);
   printf("Size in bytes: %d\n", fileMetadata.iFileSize);
   printf("Size and sum of all characters: %i\n", fileMetadata.iSumOfChars);

   free(piHash);
   free(piSizeOfFile);
   free(piSumOfChars);

   fclose(fOriginalFile);
   fclose(fOutputFile);
}
