/***************************************************************************************************************
   SOURCES

 * Title    : PG3401_Lecture_10_Network
 * Author   : Østby, B.
 * Date     : 2025-04-09
 * Slide    : 15, 16, 17
 * URL      : https://kristiania.instructure.com/courses/12804/files/1597520?module_item_id=547621

 * Title    : PG3401_Exercise_09-12_exam_preparation
 * Author   : Østby, B.
 * Date     : 2025-04-09
 * Slide    : "Client.c"
 * URL      : https://kristiania.instructure.com/courses/12804/files/1589250?module_item_id=543875

***************************************************************************************************************/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*All the code is not implemented, i have only made a connection to the server, 
but i did not recieve the file from EWA, so i assume i am using the wrong protocol.*/

int main(int iArgc, char *apszArgV[]) {
   struct sockaddr_in saAddr = {0};
   
   int iSockFd, iPort = atoi(apszArgV[4]);

   iSockFd = socket(AF_INET, SOCK_STREAM, 0);

   if(iSockFd < 0) {
      printf("ERROR opening socket\n");
      return 1;
   }

   printf("Socket is open...\n");

   saAddr.sin_family = AF_INET;
   saAddr.sin_port = htons(iPort);
   /* I just hardcoded the ip address in hexadecimal format, as I could not figure out how to
   parse the char from Args into a hexidecimal format usable here.*/
   saAddr.sin_addr.s_addr = htonl(0x7F000001);
   
   if(connect(iSockFd, (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0) {
      printf("ERROR connecting\n");
      return 1;
   }
   printf("Socket is connected...\n");

   FILE *fFile = fopen("received_file.txt", "wb");
   if(fFile == NULL) {
      printf("Failed to open file.");
   }

   int readValue;
   int szBuffer[1024];

   while((readValue = recv(iSockFd, szBuffer, 1024, 0)) > 0) {
      fwrite(szBuffer, 1, readValue, fFile);
   }

   close(iSockFd); iSockFd = -1;
   printf("Socket is closing...\n");

   return 0;
}

