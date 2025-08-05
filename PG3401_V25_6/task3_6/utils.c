#include "utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void askUser(char *szBuffer, int bufferSize) {
   fgets(szBuffer, bufferSize, stdin);
   while(szBuffer[strlen(szBuffer) - 1] == '\n') {
      szBuffer[strlen(szBuffer) - 1] = 0;
   }
}
