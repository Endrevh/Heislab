#include "management.h"
#include "hardware.h"
#include "stdlib.h"
#include <stdbool.h>

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
   bool p_orderTypes[NUMBER_OF_ORDER_TYPES];
} command;

typedef struct
{
   int floor;
   orderType order;
}request;

command Queue[HARDWARE_NUMBER_OF_FLOORS];

void queuesystemCheckButtons();

bool queuesystemRequestBetween(int floor_lower, int floor_upper);

void queuesystemRequestHandler(request* p_request);

void queuesystemDelFromQueue(int floor);

elevator_state queuesystemNewDir(); //denne må kalles ETTER 3 sekunder stopp ved etasje

void queuesystemStopAtFloor(int floor);  //sjekker om heisen skal stoppe ved gitt etasje. Må se om noen skal av, eller om noen skal på i den retningen heisen er på vei (bruk state til å sjekke dette)
                                        //kalles når vi går forbi en etasje                                       
                                        //bytte state
                                        //må aldri gå lenger ned enn nederste request, og motsatt. Dette kunne skjedd dersom man er i idle,
                                        //får en request lenger nede som skal opp. Da vil heisen begynne å gå nedover selv om det er ingen lenger nede som skal ned eller av.
                                        //foreslått løsning: sjekke om det er noen requests i etasjene under. Hvis ja, fortsett nedover. Sett opp et eksempel med dette, se hva som fungerer. Kanskje ha en bool variabel som settes til true/false når første idle-request er fullført
#endif

