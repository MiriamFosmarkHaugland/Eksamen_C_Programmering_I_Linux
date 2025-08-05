#ifndef _PLANE_H_
#define _PLANE_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "passenger.h"

typedef struct PASSENGER tPassenger;

typedef struct PLANE {
   struct PLANE* pNext;
   struct PLANE* pPrev;
   char *szFlightID;
   char *szDestination;
   int iNumOfSeats;
   int iTimeForDeparture;
   tPassenger *passengerHead;
} tPlane;

void createFlight(tPlane **head);

void insertFlightLastOnList(tPlane **head, char *szFlightID, char *szDestination, int iNumOfSeats, int iTimeForDeparture);

void deleteFlightByID(tPlane **head, char *szID);

void findFlightByID(tPlane **head);

void findDepartureDestination(tPlane **head);

void printPlaneList(tPlane *head);

void printPlane(tPlane *pstPlane);

#endif
