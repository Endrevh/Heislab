#include "management.h"
#include "hardware.h"
#include "queuesystem.h"
#include "timer.h"
#include "stdio.h"



int managementElevatorAtFloor(void) {
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if(hardware_read_floor_sensor(i)) {
            current_floor = i+1;
            return i+1;
        } 
    }
    return 0; //ingen etasje
}

void managementArrived(int floor) {

    timerReset();
    printf("KOMMET TIL ETASJE");


    for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
        hardware_command_order_light(floor, i, 0);
    }

}

void managementDepart(int floor) {
    hardware_command_door_open(0);
    queuesystemDelete(floor); 
}