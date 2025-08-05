#ifndef _UTILS_H_
#define _UTILS_H_

int serverSaveDataRecieved(void *pStructType, char *szBuffer, int iSockNewFd);

struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER createHeader(int iStructSize);

struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY stCreateServerReply(struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stHead, char *szBuffer, char szStatusCode[3], char szTextMessage[51]);

int serverWriteToSocket(char *szBuffer, int iSockNewFd);

#endif
