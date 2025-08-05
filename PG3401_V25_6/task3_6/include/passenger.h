#ifndef _PASSENGER_H_
#define _PASSENGER_H_
#include "utils.h"
#include "plane.h"

#define FALSE 0
#define TRUE 1

typedef struct PLANE tPlane;

typedef struct PASSENGER {
   struct PASSENGER* pNext;
   int iSeatNumber;
   char szName[64];
   int iAge;
} tPassenger;

typedef struct PASSENGERCOUNT {
   struct PASSENGERCOUNT* pNext;
   char szName[64];
   int iCount;
} tPassengerCount;

void printPassengerList(tPassenger *head);

void addPassenger(tPlane **head);

void changePassengerSeat(tPlane **head);

void findPassengerName(tPlane **head);

void numberOfBookingsPerPassenger(tPlane **head);

#endif
