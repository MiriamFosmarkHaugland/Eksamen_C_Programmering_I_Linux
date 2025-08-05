#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "passenger.h"
#include "plane.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main() {
   char szUserInputBuffer[64];
   char szFlightIDBuffer[64];

   tPlane *head = NULL;

   while(strcmp(szUserInputBuffer, "9") != 0) {
      printf("\n");
      printf("--------------------------------- Menu options ---------------------------------\n");
      printf("\n > (1): Add a flight. \n > (2): Add a passenger to a flight. \n > (3): Delete a flight. \n > (4): Change a passengers seat. \n > (5): See all information from a specific flight. \n > (6): See all flights to a specific destination. \n > (7): Search a passenger name, to see all their reserved flights. \n > (8): See passengers that has reserved more than one flight. \n > (9): Exit program.\n");
      printf("\n > Answer: ");

      askUser(szUserInputBuffer, sizeof(szUserInputBuffer));

      char iUserInput = atoi(szUserInputBuffer);

      if(iUserInput == 1) {
         createFlight(&head);
         printPlaneList(head);

      } else if(iUserInput == 2) {
         addPassenger(&head);

      } else if(iUserInput == 3) {
         printf("\n > Please enter the flight ID to be deleted: ");
         askUser(szFlightIDBuffer, sizeof(szFlightIDBuffer));
         deleteFlightByID(&head, szFlightIDBuffer);
         printPlaneList(head);

      } else if(iUserInput == 4) {
         changePassengerSeat(&head);

      } else if(iUserInput == 5) {
         findFlightByID(&head);

      } else if(iUserInput == 6) {
         findDepartureDestination(&head);

      } else if(iUserInput == 7) {
         findPassengerName(&head);

      } else if(iUserInput == 8) {
         numberOfBookingsPerPassenger(&head);

      } else if(iUserInput == 9) {
         return;

      } else {
         printf("\n ERROR: Not a valid option, please try again.");
      }
   }
}
