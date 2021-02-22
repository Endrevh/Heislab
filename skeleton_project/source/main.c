#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "driver/io.h"
#include "driver/channels.h"
#include "management.h"
#include "queuesystem.h"
#include "timer.h"

//bytt alle ints med bools senere
//hvis vi er i en etasje, sjekk om obstruction blir true, reset timer
//vurder å kjøre heisen til første etasje ved idle. 

static void clear_all_order_lights(){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}


int main(){

    

    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    state = STATE_STARTUP;
    clear_all_order_lights();

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");



    while(1){
        /*if(hardware_read_stop_signal()){
            printf("QWERQWER");
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
                queuesystemDelete(i+1);
            }
            state = STATE_IDLE;
            break;
        }*/

        switch (state)
        {
        case STATE_UP:
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            queuesystemStopAtFloor(managementElevatorAtFloor());
            queuesystemCheckButtons();
            break;
        
        case STATE_DOWN:
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            queuesystemStopAtFloor(managementElevatorAtFloor());
            queuesystemCheckButtons();
            break;
        case STATE_STARTUP:
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            if(managementElevatorAtFloor()) {
                state = STATE_IDLE;
            }
            break;
        case STATE_DOWN_HALT:
            //obstruction
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            if(timerTrigger(3)) {
                printf("KOMMET TIL ETASJE");
                managementDepart(current_floor);
                state = queuesystemNewDir();
            }
            queuesystemCheckButtons();
            break;
        case STATE_UP_HALT:
            //obstruction
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            if(timerTrigger(3)) {
                managementDepart(current_floor);
                state = queuesystemNewDir();
            }
            queuesystemCheckButtons();
            break;
        case STATE_IDLE:
            //obstruction??
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            queuesystemCheckButtons();
            break;
        default:
            break;
        }
        
        

        //Code block that makes the elevator go up when it reach the botton
        /*if(hardware_read_floor_sensor(0)){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }*/

        /*if(io_read_bit(BUTTON_UP1) == 1){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }*/

        /* Code block that makes the elevator go down when it reach the top floor*/
        /*if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            state = STATE_DOWN;
        }*/

        /* All buttons must be polled, like this: */
        
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                hardware_command_floor_indicator_on(f);
            }
        }

        /* Lights are set and cleared like this: */
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            //Internal orders
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            }

            //Orders going up
            if(hardware_read_order(f, HARDWARE_ORDER_UP)){
                hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            }

            //Orders going down
            if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
                hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            }
        }

        /* Code to clear all lights given the obstruction signal */
        if(hardware_read_obstruction_signal()){
            hardware_command_stop_light(1);
            clear_all_order_lights();
        }
        else{
            hardware_command_stop_light(0);
        }
    }

    return 0;
}
