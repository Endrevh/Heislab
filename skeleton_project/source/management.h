#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#define FLOOR_WAIT_TIME 3

typedef enum {
    STATE_STARTUP = 1,
    STATE_UP,
    STATE_UP_HALT,
    STATE_DOWN,
    STATE_DOWN_HALT,
    STATE_IDLE,
    STATE_EMERGENCY_STOP
} elevator_state;

elevator_state state;
int current_floor;

//slette requests fra queue
//starte timer
//slå av lys på tilhørende requests 
//åpne døren, slå på dørlys
void managementArrived(int floor); 

void managementDepart(int floor);

int managementElevatorAtFloor(void); //sjekker om heisen er ved en etasje. 




#endif