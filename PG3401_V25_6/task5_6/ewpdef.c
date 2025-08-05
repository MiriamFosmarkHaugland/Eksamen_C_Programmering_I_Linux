/***************************************************************************************************************
   SOURCES

 * Title    : PG3401_Lecture_10_Network
 * Author   : Østby, B.
 * Date     : 2025-04-03
 * Slide     : 10, 11, 12, 13, 20, 22, 24
 * URL      : https://kristiania.instructure.com/courses/12804/files/1597520?module_item_id=547621

 * Title    : PG3401_Exercise_09-12_exam_preparation
 * Author   : Østby, B.
 * Date     : 2025-04-03
 * Slide    : "Normal Client-Server app #2", "Server.c", "Client.c"
 * URL      : https://kristiania.instructure.com/courses/12804/files/1589250?module_item_id=543875

 * Title    : Socket Programming in C
 * Author   : GeeksForGeeks.org
 * Date     : 2025-04-23
 * Accessed : 2025-05-05
 * URL      : https://www.geeksforgeeks.org/socket-programming-cc/

 * Title    : How can i get the date and time values in a C program
 * Author   : Adam Rosenfield
 * Date     : 2019-12-30
 * Accessed : 2025-05-06
 * URL      : https://stackoverflow.com/questions/5589136/copy-struct-to-buffer

 * Title    : How recv() function works when looping
 * Author   : user207421
 * Date     : 2024-07-30
 * Accessed : 2025-05-07
 * URL      : https://stackoverflow.com/questions/27205810/how-recv-function-works-when-looping

***************************************************************************************************************/

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

#define STATUS_CODE_OK "250"
#define STATUS_CODE_CLOSED "221"
#define STATUS_CODE_READY "354"
#define STATUS_CODE_SERVER_IS_READY "220"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int iArgc, char *apszArgv[]) {

   struct sockaddr_in saAddr = {0};
   struct sockaddr_in saConClient = {0};

   int iSockFd, iSockNewFd = 0;
   int addrLen = sizeof(saAddr);
   char szBuffer[64];

   iSockFd = socket(AF_INET, SOCK_STREAM, 0);
   if(iSockFd < 0) {
      printf("ERROR opening socket");
      return 1;
   }
   printf("Socket is open...\n");

   int iPort = atoi(apszArgv[2]);

   saAddr.sin_family = AF_INET;
   saAddr.sin_port = htons(iPort);
   saAddr.sin_addr.s_addr = INADDR_ANY;

   if(bind(iSockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
      printf("ERROR on binding with %i\n", errno);
      return 1;
   }
   
   listen(iSockFd, 5);
   
   iSockNewFd = accept(iSockFd, (struct sockaddr *) &saConClient, (socklen_t*) &addrLen);
   if(iSockNewFd < 0) {
      printf("ERROR on accept");
      return 1;
   } else {
      printf("Socket has accepted...\n");

      time_t t;
      struct tm *tmp;
      char szTime[21];
      time(&t);
      tmp = localtime(&t);
      strftime(szTime, sizeof(szTime), "%d %b %Y, %H:%M:%S", tmp);


      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stAcceptHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERACCEPT));
  
      struct EWA_EXAM25_TASK5_PROTOCOL_SERVERACCEPT stServerAccept;
      stServerAccept.stHead = stAcceptHead;
      
      memcpy(stServerAccept.acStatusCode, STATUS_CODE_SERVER_IS_READY, 3);
      stServerAccept.acHardSpace[0] = ' ';

      char szFormattedString[51];
      int writtenBytes = sprintf(szFormattedString, "%s %s %s %s", "127.0.0.1", "SMTP", apszArgv[4], szTime);

      memset(szFormattedString + writtenBytes, 20, 51 - writtenBytes);

      memcpy(stServerAccept.acFormattedString, szFormattedString, 51);
      stServerAccept.acHardZero[0] = '\0';
      
      memcpy(szBuffer, &stServerAccept, sizeof(stServerAccept));

      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }


      struct EWA_EXAM25_TASK5_PROTOCOL_CLIENTHELO stClientHelo;

      if(serverSaveDataRecieved(&stClientHelo, szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stHeloHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERHELO));

      struct EWA_EXAM25_TASK5_PROTOCOL_SERVERHELO stServerHelo;
      stServerHelo.stHead = stHeloHead;
      memcpy(stServerHelo.acStatusCode, STATUS_CODE_OK, 3);
      stServerHelo.acHardSpace[0] = ' ';
      stServerHelo.acHardZero[0] = '\0';

      char *name = strtok(stClientHelo.acFormattedString, ".");
      char *address = strtok(NULL, "");
   
      writtenBytes = sprintf(szFormattedString, "%.15s Hello %.15s", address, name);

      memset(szFormattedString + writtenBytes, 20, 51 - writtenBytes);
      memcpy(stServerHelo.acFormattedString, szFormattedString, 51);
      memcpy(szBuffer, &stServerHelo, sizeof(stServerHelo));
   
      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }


      struct EWA_EXAM25_TASK5_PROTOCOL_MAILFROM stMailFrom;

      if(serverSaveDataRecieved(&stMailFrom, szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stReplyMailHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY));

      stCreateServerReply(stReplyMailHead, szBuffer, STATUS_CODE_OK, "Sender address ok");
   
      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      
      struct EWA_EXAM25_TASK5_PROTOCOL_RCPTTO stRcpTo;

      if(serverSaveDataRecieved(&stRcpTo, szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stReplyRcptHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY));

      stCreateServerReply(stReplyRcptHead, szBuffer, STATUS_CODE_OK, "Recipent address ok");
   
      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }


      struct EWA_EXAM25_TASK5_PROTOCOL_CLIENTDATACMD stClientDataCmd;

      if(serverSaveDataRecieved(&stClientDataCmd, szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stReplyFileNameHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY));

      stCreateServerReply(stReplyFileNameHead, szBuffer, STATUS_CODE_READY, "Ready for message");
   
      char *fileName = stClientDataCmd.acFormattedString;
   
      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      
      struct EWA_EXAM25_TASK5_PROTOCOL_CLIENTDATAFILE stClientDataFile;

      char szLargeBuffer[9998];
      int iBytesTotalReceived = 0;
      char szEndMarker[] = "\r\n\r\n.\r\n";
      int iDone = 0;

      while(1) {
         int iBytesReceived = recv(iSockNewFd, szLargeBuffer + iBytesTotalReceived, sizeof(szLargeBuffer), 0);

         iBytesTotalReceived = iBytesTotalReceived + iBytesReceived;

         if(iBytesReceived == -1) {
            printf("ERROR on reading...\n");
            return 1;
         }
         for(int i = iBytesTotalReceived - 7; i >= 0; i--) {
            if(memcmp(szLargeBuffer + i, szEndMarker, 7) == 0) {
               iDone = 1;
               break;
            }
         }
         if(iDone == 1) {
            break;
         }    
      }

      memcpy(&stClientDataFile, szLargeBuffer, 9998);

      FILE *fFile = fopen(fileName, "wb");
      if(fFile == NULL) {
         printf("ERROR opening file");
         return 1;
      }
      fprintf(fFile, szLargeBuffer);

      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stReplyFileHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY));

      stCreateServerReply(stReplyFileHead, szBuffer, STATUS_CODE_OK, "Message accepted");

      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      fclose(fFile);


      struct EWA_EXAM25_TASK5_PROTOCOL_CLOSECOMMAND stCloseCommand;

      if(serverSaveDataRecieved(&stCloseCommand, szBuffer, iSockNewFd) == 1) {
         return 1;
      }

      struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stReplyQuitHead = createHeader(sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY));
      char message[64];
      sprintf(message, "%.15s closing connection", address);
      stCreateServerReply(stReplyQuitHead, szBuffer, STATUS_CODE_CLOSED, message);

      if(serverWriteToSocket(szBuffer, iSockNewFd) == 1) {
         return 1;
      }    
    
      printf("Socket is closing...\n");
      close(iSockNewFd); iSockFd = -1;
      close(iSockFd); iSockFd = -1;
   }
   return 0;
}
