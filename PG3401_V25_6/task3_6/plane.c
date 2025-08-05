/***************************************************************************************************************
   SOURCES

 * Title    : The C Programming Language
 * Author   : Kernighan, B. W & Ritchie, D. M
 * Date     : 1988
 * Page     : 135
 * Publisher: Prentice Hall
 * URL      : https://colorcomputerarchive.com/repo/Documents/Books/The%20C%20Programming%20Language%20%28Kernighan%20Ritchie%29.pdf

 * Title    : Doubly Linked List in C
 * Author   : GeeksForGeeks.org
 * Date     : 2024-10-25
 * Accessed : 2025-05-01
 * URL      : https://www.geeksforgeeks.org/doubly-linked-list-in-c/

***************************************************************************************************************/

#include "plane.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createFlight(tPlane **head) {
   char szFlightIDBuffer[64];
   char szDestinationBuffer[64];
   char szNumOfSeatsBuffer[64];
   char szTimeForDepartureBuffer[64];

   printf("\n----------------------------- Welcome to C airport -----------------------------\n");
   printf("\n - Follow the steps below to create a flight\n");
   printf("\n");

   printf(" > Please enter your flight ID: ");
   askUser(szFlightIDBuffer, sizeof(szFlightIDBuffer));

   printf(" > Please enter your flight destination: ");
   askUser(szDestinationBuffer, sizeof(szDestinationBuffer));

   printf(" > Please enter your amount of flight seats: ");
   askUser(szNumOfSeatsBuffer, sizeof(szNumOfSeatsBuffer));

   printf(" > Please enter your flight time for departure: ");
   askUser(szTimeForDepartureBuffer, sizeof(szTimeForDepartureBuffer));

   insertFlightLastOnList(head, szFlightIDBuffer, szDestinationBuffer, atoi(szNumOfSeatsBuffer), atoi(szTimeForDepartureBuffer));
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void insertFlightLastOnList(tPlane **head, char *szFlightID, char *szDestination, int iNumOfSeats, int iTimeForDeparture) {

   tPlane *pstNewFlight = (tPlane *)malloc(sizeof(tPlane));
   if(pstNewFlight == NULL) {
      printf("Memory allocation for the new flight failed\n");
      free(pstNewFlight);
      return;
   }

   pstNewFlight->szFlightID = (char *)malloc(strlen(szFlightID) + 1);
   if(pstNewFlight->szFlightID == NULL) {
      printf("Memory allocation for the flight ID failed\n");
      free(pstNewFlight);
      return;
   }

   pstNewFlight->szDestination = (char *)malloc(strlen(szDestination) + 1);
   if(pstNewFlight->szDestination == NULL) {
      printf("Memory allocation for the flight destination failed\n");
      free(pstNewFlight);
      return;
   }

   // Store unique copy of the flight ID and destination.
   strcpy(pstNewFlight->szFlightID, szFlightID);
   strcpy(pstNewFlight->szDestination, szDestination);

   // Setting the properties on pstNewFlight equal to the function parameters.
   pstNewFlight->iNumOfSeats = iNumOfSeats;
   pstNewFlight->iTimeForDeparture = iTimeForDeparture;
   pstNewFlight->pNext = NULL;

   // If the list is empty we set pstNewFlight as the first item.
   if(*head == NULL) {
      pstNewFlight->pPrev = NULL;
      *head = pstNewFlight;
      return;
   }

   // Traverse to the end of the list when it is not empty.
   tPlane *pTmp = *head;
   while(pTmp->pNext != NULL) {
      pTmp = pTmp->pNext;
   }

   // Insert a new flight at the end.
   pTmp->pNext = pstNewFlight;
   pstNewFlight->pPrev = pTmp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void deleteFlightByID(tPlane **head, char *szID) {
   int iFoundFlighIDMatch = 0;

   if(head == NULL) {
      return;
   }

   tPlane *pstPlane = *head;

   while(pstPlane != NULL) {
      if(strcmp(pstPlane->szFlightID, szID) == 0) {
         iFoundFlighIDMatch = 1;
         tPlane *pstPlanePrev = pstPlane->pPrev;
         tPlane *pstPlaneNext = pstPlane->pNext;

         // We update the head variable, since we are deleting the previous head.
         if(pstPlane == *head) {
            *head = pstPlaneNext;
         }

         if(pstPlanePrev != NULL) {
            pstPlanePrev->pNext = pstPlaneNext;
         }

         if(pstPlaneNext != NULL) {
            pstPlaneNext->pPrev = pstPlanePrev;
         }

         tPassenger *pTmpPassenger = pstPlane->passengerHead;
         while (pTmpPassenger != NULL) {
            tPassenger *pNextPassenger = pTmpPassenger->pNext;
            printf("\n - Passenger '%s' was successfully deleted.\n", pTmpPassenger->szName);
            free(pTmpPassenger);
            pTmpPassenger = pNextPassenger;
         }

         free(pstPlane);

         pstPlane = pstPlaneNext;
         printf("\n - Flight was successfully deleted.\n");
         break;
      } else {
         pstPlane = pstPlane->pNext;
      }
   }

   if(iFoundFlighIDMatch == 0) {
      printf("\nERROR: Could not find a match for the ID, please try again.\n");
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findFlightByID(tPlane **head) {
   char szFlightIDBuffer[64];

   int iFoundFlightIDMatch = 0;

   if(head == NULL) {
      return;
   }

   tPlane *pstPlane = *head;

   printf("\n > Please enter the flight ID: ");
   askUser(szFlightIDBuffer, sizeof(szFlightIDBuffer));

   while(pstPlane != NULL) {
      if(strcmp(pstPlane->szFlightID, szFlightIDBuffer) == 0) {
         iFoundFlightIDMatch = 1;
         printf("\n - Here are all information about flight with ID %s.\n", pstPlane->szFlightID);
         printPlane(pstPlane);
         printPassengerList(pstPlane->passengerHead);
         break;
      } else {
         pstPlane = pstPlane->pNext;
      }
   }

   if(iFoundFlightIDMatch == 0) {
      printf("\nERROR: Could not find a match for the ID, please try again.\n");
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findDepartureDestination(tPlane **head) {
   char szFlightIDBuffer[64];
   char szDestinationBuffer[64];

   int iFoundFlightDestinationMatch = 0;

   tPlane *pstPlane = *head;

   printf("\n > Please enter a destination: ");
   askUser(szDestinationBuffer, sizeof(szDestinationBuffer));

   printf("\n - Here are all flights going to %s.\n", szDestinationBuffer);

   while(pstPlane != NULL) {
      if(strcmp(pstPlane->szDestination, szDestinationBuffer) == 0) {
         iFoundFlightDestinationMatch = 1;
         printPlane(pstPlane);
      }
      pstPlane = pstPlane->pNext;
   }

   if(iFoundFlightDestinationMatch == 0) {
      printf("\nERROR: Could not find an available plane for this destination, please try again.\n");
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printPlaneList(tPlane *head) {
   tPlane *pstPlane = head;
   if(head == NULL) {
      printf("\n");
      printf("----------------------------- Overview of flights -----------------------------\n");
      printf(" - No flights found.");
      return;
   }

   printf("\n");
   printf("----------------------------- Overview of flights -----------------------------\n");

   while(pstPlane != NULL) {
      printPlane(pstPlane);
      printPassengerList(pstPlane->passengerHead);
      pstPlane = pstPlane->pNext;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printPlane(tPlane *pstPlane) {
   printf("\n");
   printf("- Flight ID: %s\n", pstPlane->szFlightID);
   printf("- Flight destination: %s\n", pstPlane->szDestination);
   printf("- Number of seats: %d\n", pstPlane->iNumOfSeats);
   printf("- Time for departure: %d\n", pstPlane->iTimeForDeparture);
}
