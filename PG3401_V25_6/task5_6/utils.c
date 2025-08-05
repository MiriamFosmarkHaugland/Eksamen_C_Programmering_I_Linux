#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>

#include "ewpdef.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int serverSaveDataRecieved(void *pStructType, char *szBuffer, int iSockNewFd) {
   memset(szBuffer, 0, 64);

   int readValue = recv(iSockNewFd, szBuffer, 64, MSG_WAITALL);
   
   if(readValue < 0) {
      printf("ERROR on reading...\n");
      return 1;
   }

   memcpy(pStructType, szBuffer, 64);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER createHeader(int iStructSize) {
   struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stHeader;

   char szDataSize[4];

   memcpy(stHeader.acMagicNumber, "EWP", 3);
   sprintf(szDataSize, "00%d", iStructSize);
   memcpy(stHeader.acDataSize, szDataSize, 4);
   memcpy(stHeader.acDelimeter, "|", 1);

   return stHeader;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY stCreateServerReply(struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stHead, char *szBuffer, char szStatusCode[3], char szTextMessage[51]) {

   struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY stServer;
   stServer.stHead = stHead;

   char szFormattedString[51];
   
   memcpy(stServer.acStatusCode, szStatusCode, 3);
   stServer.acHardSpace[0] = ' ';
   stServer.acHardZero[0] = '\0';
   
   int writtenBytes = sprintf(szFormattedString, szTextMessage);

   memset(szFormattedString + writtenBytes, 20, 51 - writtenBytes);
   memcpy(stServer.acFormattedString, szFormattedString, 51);
   memcpy(szBuffer, &stServer, sizeof(stServer));

   return stServer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int serverWriteToSocket(char *szBuffer, int iSockNewFd) {
   int writeToSocket = send(iSockNewFd, szBuffer, strlen(szBuffer) + 1, 0);
   printf("Socket has written...\n");
   if(writeToSocket < 0) {
      printf("ERROR writing to socket");
      return 1;
   }
   return 0;
}
