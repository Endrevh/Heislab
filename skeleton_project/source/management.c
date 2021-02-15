#include "management.h"
#include "hardware.h"
#include "queuesystem.h"


state = STATE_STARTUP;

int managementElevatorAtFloor(void) {
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if(hardware_read_floor_sensor(i)) {
            current_floor = i+1;
            return i+1;
        } 
    }
    return 0; //ingen etasje
}

void managementStopAtFloor(int floor) {
    if(!floor) return;

    switch (state)
    {
    case STATE_UP:
        if(Queue[floor-1].p_orderTypes[ORDER_UP] == 1 || Queue[floor-1].p_orderTypes[ORDER_INSIDE] == 1) {
            state = STATE_UP_HALT;
            managementArrived(floor);
        }
        break;
    case STATE_DOWN:
        if(Queue[floor-1].p_orderTypes[ORDER_DOWN] == 1 || Queue[floor-1].p_orderTypes[ORDER_INSIDE] == 1) {
            state = STATE_DOWN_HALT;
            managementArrived(floor);
        }
        break;
    default:
        break;
    }
}