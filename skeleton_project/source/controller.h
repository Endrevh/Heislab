#pragma once
#include "hardware.h"
#include "timer.h"
#include "stdlib.h"
#include <stdbool.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H
#define NUMBER_OF_ORDER_TYPES 3
#define FLOOR_WAIT_TIME 3

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

typedef enum {
    STATE_STARTUP = 1,
    STATE_UP,
    STATE_UP_HALT,
    STATE_DOWN,
    STATE_DOWN_HALT,
    STATE_IDLE,
    STATE_EMERGENCY_STOP
} elevator_state;

typedef enum {
    EMERGENCY_HANDLED,
    EMERGENCY_ABOVE,
    EMERGENCY_BELOW
} emergency_types;

elevator_state state;
emergency_types emergency_status;
int current_floor;

command Queue[HARDWARE_NUMBER_OF_FLOORS];

void controllerCheckOrderBtns();

bool controllerRequestBetween(int floor_lower, int floor_upper);

void controllerRequestHandler(request* p_request);

void controllerDelFromQueue(int floor);

elevator_state controllerNewDir(); //denne må kalles ETTER 3 sekunder stopp ved etasje

void controllerStopAtFloor(int floor);  //sjekker om heisen skal stoppe ved gitt etasje. Må se om noen skal av, eller om noen skal på i den retningen heisen er på vei (bruk state til å sjekke dette)
                                        //kalles når vi går forbi en etasje                                       
                                        //bytte state
                                        //må aldri gå lenger ned enn nederste request, og motsatt. Dette kunne skjedd dersom man er i idle,
                                        //får en request lenger nede som skal opp. Da vil heisen begynne å gå nedover selv om det er ingen lenger nede som skal ned eller av.
                                        //foreslått løsning: sjekke om det er noen requests i etasjene under. Hvis ja, fortsett nedover. Sett opp et eksempel med dette, se hva som fungerer. Kanskje ha en bool variabel som settes til true/false når første idle-request er fullført


//slette requests fra queue
//starte timer
//slå av lys på tilhørende requests 
//åpne døren, slå på dørlys
void controllerArrived(); 

void controllerDepart();

void controllerEmergencyHandler();

/**
 * @brief Checks if the elevator is at a floor
 * @return floor number if success, 0 if no floor
*/
int controllerElevatorAtFloor(void);

#endif

