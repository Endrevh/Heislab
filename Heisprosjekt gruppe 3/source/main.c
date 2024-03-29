#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "driver/io.h"
#include "driver/channels.h"
#include "controller.h"
#include "timer.h"

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
    emergency_status = EMERGENCY_HANDLED;

    clear_all_order_lights();
    hardware_command_door_open(0);

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");




    while(1){

        if(hardware_read_stop_signal() && state!=STATE_STARTUP){
            timerReset(&timer);
            clear_all_order_lights();
            hardware_command_stop_light(1);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
                controllerDelFromQueue(i+1);
            }
            controllerEmergencyHandler();
        }

        switch (state)
        {
        case STATE_UP:
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            controllerStopAtFloor(controllerElevatorAtFloor());
            controllerCheckOrderBtns();
            break;
        
        case STATE_DOWN:
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            controllerStopAtFloor(controllerElevatorAtFloor());
            controllerCheckOrderBtns();
            break;
        case STATE_STARTUP:
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            if(controllerElevatorAtFloor()) {
                state = STATE_IDLE;
            }
            break;
        case STATE_DOWN_HALT:
            if(hardware_read_obstruction_signal()){
                timerReset(&timer);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            if(timerTrigger(&timer, FLOOR_WAIT_TIME)) {
                controllerDepart();
                state = controllerNewDir();
            }
            controllerCheckOrderBtns();
            break;
        case STATE_UP_HALT:
            if(hardware_read_obstruction_signal()){
                timerReset(&timer);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            if(timerTrigger(&timer, FLOOR_WAIT_TIME)) {
                controllerDepart();
                state = controllerNewDir();
            }
            controllerCheckOrderBtns();
            break;
        case STATE_IDLE:
            if(timerTrigger(&timer, FLOOR_WAIT_TIME)) {
                hardware_command_door_open(0);
            } 
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            controllerCheckOrderBtns();
            clear_all_order_lights();
            break;
        case STATE_EMERGENCY_STOP:
            if(controllerElevatorAtFloor()){
                if(timerTrigger(&timer, FLOOR_WAIT_TIME)) {
                    hardware_command_door_open(0);
                }
                else{
                    hardware_command_door_open(1);
                }
            }
            if(!hardware_read_stop_signal()) {
                hardware_command_stop_light(0);
                controllerCheckOrderBtns();}
            break;
        default:
            break;
        }
       

        /* All buttons must be polled, like this: */
        
        for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_floor_sensor(f)){
                hardware_command_floor_indicator_on(f);
            }
        }

        /* Lights are set and cleared like this: */
        if(!(state == STATE_STARTUP)) {
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
        }
       
    }

    return 0;
}
