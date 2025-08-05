/***************************************************************************************************************
  SOURCES
 
 * Title    : C Program to Implement Singly Linked List
 * Author   : GeeksForGeeks.org
 * Date     : 2024-06-19
 * Accessed : 2025-05-01
 * URL      : https://www.geeksforgeeks.org/c-program-to-implement-singly-linked-list/

***************************************************************************************************************/

#include "passenger.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printPassengerList(tPassenger *head) {
  tPassenger *pstPassenger = head;
  if(head == NULL) {
     printf("\n");
     printf("\t- No passengers currently on this flight.\n");
     return;
   }

   while(pstPassenger != NULL) {
      printf("\n");
      printf("\t- Passenger name: %s\n", pstPassenger->szName);
      printf("\t- Seat number: %d\n", pstPassenger->iSeatNumber);
      printf("\t- Passenger age: %d\n", pstPassenger->iAge);

      pstPassenger = pstPassenger->pNext;
   } 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addPassenger(tPlane **head) {
   char szSeatNumberBuffer[4];
   char szNameBuffer[64];
   char szAgeBuffer[4];
   char szFlighIDBuffer[64];

   int iFoundFlighIDMatch = 0;

   printPlaneList(*head);

   if(head == NULL) {
      return;
   }

   printf("\n > Please enter the flight ID of the flight you wanna take: ");
   askUser(szFlighIDBuffer, sizeof(szFlighIDBuffer));

   tPlane *pstPlane = *head;

   while(pstPlane != NULL) {
      if(strcmp(pstPlane->szFlightID, szFlighIDBuffer) == 0) {
         iFoundFlighIDMatch = 1;

         printf(" > Please enter your seat number: ");
         askUser(szSeatNumberBuffer, sizeof(szSeatNumberBuffer));

         if(atoi(szSeatNumberBuffer) > pstPlane->iNumOfSeats ) {
            printf("\nERROR: This seat number is too high, please try again.\n");
            return;
         }
         
         printf(" > Please enter your name: ");
         askUser(szNameBuffer, sizeof(szNameBuffer));

         printf(" > Please enter your age: ");
         askUser(szAgeBuffer,  sizeof(szAgeBuffer));
         break;
      } else {
         pstPlane = pstPlane->pNext;
      }
   }

   if(iFoundFlighIDMatch == 0) {
      printf("\nERROR: Could not find a match for the ID, please try again.\n");
      return;
   }

   tPassenger *pstNewPassenger = (tPassenger *)malloc(sizeof(tPassenger));
   if(pstNewPassenger == NULL) {
      printf("Memory allocation for the new passenger failed\n");
      free(pstNewPassenger);
      return;
   }

   strcpy(pstNewPassenger->szName, szNameBuffer);

   pstNewPassenger->iSeatNumber = atoi(szSeatNumberBuffer);
   pstNewPassenger->iAge = atoi(szAgeBuffer);

   if(pstPlane->passengerHead == NULL) {
      // There are no passengers on the plane yet so we add the first one.
      pstPlane->passengerHead = pstNewPassenger;
   } else {
      // Loop through the head until we find one with a seatnumber higher than the current passenger.
      tPassenger *pPassenger = pstPlane->passengerHead;
      while(pPassenger != NULL) {
          if(pPassenger->iSeatNumber < pstNewPassenger->iSeatNumber) {
             if(pPassenger->pNext == NULL) {
                // If there is no next passenger add the new passenger.
                pPassenger->pNext = pstNewPassenger;
                break;
             }

             if(pPassenger->pNext->iSeatNumber < pstNewPassenger->iSeatNumber) {
                pPassenger = pPassenger->pNext;
             } else {
                // The current passengers seat number is less than the new passenger and the next passengers seat number is higher than the new passenger.
                pstNewPassenger->pNext = pPassenger->pNext;
                pPassenger->pNext = pstNewPassenger;
                break;
             }
          } else {
            // If the seat number is the lowest, we set it as the new head and point to the old head as next.
            pstNewPassenger->pNext = pPassenger;
            pstPlane->passengerHead = pstNewPassenger;
            break;
          }
      }
   }
   printf(" \n- Passenger was successfully added.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void numberOfBookingsPerPassenger(tPlane **head) {
   if(*head == NULL) {
     printf("\n");
     printf("\t- No passengers currently on any flights.\n");
     return;
   }

   tPlane *pstPlane = *head;
   tPassengerCount *pstCountHead = NULL;
   
   while(pstPlane != NULL) {
      tPassenger *pstPassenger = pstPlane->passengerHead;
      while(pstPassenger != NULL) {
         if(pstCountHead == NULL) {
            // If we have no counter in our linked list add the head of the list.
            pstCountHead = (tPassengerCount *)malloc(sizeof(tPassengerCount));
            strcpy(pstCountHead->szName, pstPassenger->szName);
            pstCountHead->iCount = 1;
            pstCountHead->pNext = NULL;
         } else {
            // If we do have a head, look for a passenger name in the counter list.
            int bFoundPassenger = FALSE;
            tPassengerCount *pstLastPassenger = pstCountHead;
            tPassengerCount *pstTmpCount = pstCountHead;
            while(pstTmpCount != NULL) {
               if(strcmp(pstTmpCount->szName, pstPassenger->szName) == 0) {
                  // If we find a counter with the name of the passenger we update the count by 1.
                  pstTmpCount->iCount = pstTmpCount->iCount + 1;
                  bFoundPassenger = TRUE;
                  break;
               }
               pstLastPassenger = pstTmpCount;
               pstTmpCount = pstTmpCount->pNext;
            }
            if(bFoundPassenger == FALSE) {
               // If we are at the end of the counter list and did not find a passenger in the list, we add the passenger at the end of the list.
               tPassengerCount *pstNewPassenger = (tPassengerCount *)malloc(sizeof(tPassengerCount));
               strcpy(pstNewPassenger->szName, pstPassenger->szName);
               pstNewPassenger->iCount = 1;
               pstNewPassenger->pNext = NULL;
               pstLastPassenger->pNext = pstNewPassenger;
            }
         }
         pstPassenger = pstPassenger->pNext;
      }
      pstPlane = pstPlane->pNext;
   }

   tPassengerCount *pstTmpPassenger = pstCountHead;

   if(pstTmpPassenger->iCount <= 1) {
      printf("\t - No passengers has booked more than 1 flight.\n");
   }

   while(pstTmpPassenger != NULL && pstTmpPassenger->iCount > 1) {
      printf("\n - %s is going on %d flights.\n", pstTmpPassenger->szName, pstTmpPassenger->iCount);
      tPassengerCount *pstNextPassenger = pstTmpPassenger->pNext;
      free(pstTmpPassenger);
      pstTmpPassenger = pstNextPassenger;
   }
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void changePassengerSeat(tPlane **head) {
   char szFlightIDBuffer[64];
   char szNameBuffer[64];
   char szSeatNumberBuffer[4];
   char szNewSeatNumberBuffer[4];

   tPlane *pstPlane = *head;

   printf("\n > Please enter the flight ID for the seat you want to change: ");
   askUser(szFlightIDBuffer, sizeof(szFlightIDBuffer));
   
   int bPlaneFound = FALSE;
   while(pstPlane != NULL) {
      if(strcmp(szFlightIDBuffer, pstPlane->szFlightID) != 0) {
         pstPlane = pstPlane->pNext;
      } else {
         bPlaneFound = TRUE;
         break;
      }
   }
   
   if(bPlaneFound == FALSE) {
      printf("\nERROR: We could not find the flight ID, please try again.\n");
      return;
   }

   printf(" > Please enter the name of the person of the reservation: ");
   askUser(szNameBuffer, sizeof(szNameBuffer));

   int bPassengerFound = FALSE;
   tPassenger *pstTmpPassenger = pstPlane->passengerHead;
   while(pstTmpPassenger != NULL) {
      if(strcmp(szNameBuffer, pstTmpPassenger->szName) != 0) {
         pstTmpPassenger = pstTmpPassenger->pNext;
      } else {
         bPassengerFound = TRUE;
         break;
      }
   }

   if(bPassengerFound == FALSE) {
      printf("\nERROR: We could not find your name, please try again.\n");
      return;
   }

   printf(" > Please enter your new seat number: ");
   askUser(szNewSeatNumberBuffer, sizeof(szNewSeatNumberBuffer));
   
   if(atoi(szNewSeatNumberBuffer) > pstPlane->iNumOfSeats ) {
      printf("\nERROR: This seat number is too high, please try again.\n");
      return;
   }

   pstTmpPassenger->iSeatNumber = atoi(szNewSeatNumberBuffer);

   printf("\n - Seat number was successfully changed.\n");

   printPlane(pstPlane);
   printPassengerList(pstPlane->passengerHead);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findPassengerName(tPlane **head) {
   char szNameBuffer[64];

   int iFoundNameMatch = 0;

   tPlane *pstPlane = *head;

   printf("\n > Please enter the name of the passenger: ");
   askUser(szNameBuffer, sizeof(szNameBuffer));

   printf("\n - Here are all flights %s has booked.\n", szNameBuffer);

   while(pstPlane != NULL) {
      tPassenger *pstTmpPassenger = pstPlane->passengerHead;
      while(pstTmpPassenger != NULL) {
         if(strcmp(pstTmpPassenger->szName, szNameBuffer) == 0) {
            iFoundNameMatch = 1;
            printPlane(pstPlane);
         }
         pstTmpPassenger = pstTmpPassenger->pNext;
      }
      
      pstPlane = pstPlane->pNext;
   }

   if(iFoundNameMatch == 0) {
      printf("\nERROR: Could not find the passenger name on any planes, please try again.\n");
   }
}
