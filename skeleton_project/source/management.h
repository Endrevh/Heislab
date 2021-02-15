#ifndef MANAGEMENT_H
#define MANAGEMENT_H

typedef enum {
    STATE_STARTUP = 1,
    STATE_UP,
    STATE_UP_HALT,
    STATE_DOWN,
    STATE_DOWN_HALT,
    STATE_IDLE
} elevator_state;

elevator_state state;
int current_floor;

void managementArrived(int floor); //slukke lys (se main.c) og slette fra kø

void managementDepart(void);

int managementElevatorAtFloor(void); //sjekker om heisen er ved en etasje. 

void managementStopAtFloor(int floor); //sjekker om heisen skal stoppe ved gitt etasje. Må se om noen skal av, eller om noen skal på i den retningen heisen er på vei (bruk state til å sjekke dette)
                                        //kalles når vi går forbi en etasje                                       
                                        //bytte state
                                       


#endif