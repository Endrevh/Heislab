#include "management.h"
#include "hardware.h"

#ifndef QUEUESYSTEM_H
#define QUEUESYSTEM_H
#define NUMBER_OF_ORDER_TYPES 3

typedef enum {
    ORDER_UP = 0,
    ORDER_INSIDE,
    ORDER_DOWN
} orderType;

typedef struct
{
   int p_orderTypes[NUMBER_OF_ORDER_TYPES];
} command;

typedef struct
{
   int floor;
   orderType order;
}request;

command Queue[HARDWARE_NUMBER_OF_FLOORS];


void queueRequestHandler(request* p_request);

void queueDelete(int floor);

elevator_state queueNewDirection(); 

#endif

