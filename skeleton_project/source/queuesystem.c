#include "queuesystem.h"
#include "timer.h"


#include "stdio.h"


void queuesystemRequestHandler(request* p_request) { //hvis vi er i idle, settes direction (state) med en gang i retning etasjen vi fikk request fra
    int requested_floor = p_request->floor;
    orderType order = p_request->order;
    switch (state)
    {
    case STATE_IDLE:
            if(requested_floor == current_floor){
                timerReset();
                hardware_command_door_open(1);
                for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
                    hardware_command_order_light(requested_floor-1, i, 0);
                }
            }
            else if(requested_floor < current_floor){
                state = STATE_DOWN;
                managementDepart(current_floor);
            }
            else{
                state = STATE_UP;
                managementDepart(current_floor);
            }
        break;
    case STATE_EMERGENCY_STOP:
        if(managementElevatorAtFloor() && requested_floor == current_floor) {
            state = STATE_IDLE;
            hardware_command_door_open(1);
            timerReset();
        } 
        else if(requested_floor <= current_floor) {
            state = STATE_DOWN;
            hardware_command_door_open(0);
        }
        else {
            state = STATE_UP;
            hardware_command_door_open(0);
        }
    default:
        break;
    }
    Queue[requested_floor-1].p_orderTypes[order] = 1;
}

void queuesystemDelete(int floor) {
    for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
            Queue[floor-1].p_orderTypes[i] = 0;
    }
}


void queuesystemStopAtFloor(int floor) {
    if(!floor) return;
    int requestsAbove = queuesystemRequestBetween(floor+1, HARDWARE_NUMBER_OF_FLOORS);
    int requestsBelow = queuesystemRequestBetween(1, floor-1);
    switch (state)
    {
    case STATE_UP:
        //sjekker for requests fra etasjer over
        //stopper hvis det et ikke er noen requests over, eller noen skal av eller oppover
        if(Queue[floor-1].p_orderTypes[ORDER_UP] == 1 || Queue[floor-1].p_orderTypes[ORDER_INSIDE] == 1 || !requestsAbove) {
            state = STATE_UP_HALT;
            managementArrived(floor);
        }
        break;
    case STATE_DOWN:
        //sjekker for requests fra etasjer lenger nede
        //stopper hvis det ikke er noen requests under, eller noen skal av eller nedover
        if(Queue[floor-1].p_orderTypes[ORDER_DOWN] == 1 || Queue[floor-1].p_orderTypes[ORDER_INSIDE] == 1 || !requestsBelow) {
            state = STATE_DOWN_HALT;
            managementArrived(floor);
        }
        break;
    default:
        break;
    }
}

int queuesystemRequestBetween(int floor_lower, int floor_upper) {
    int requestsBetween = 0;
    for(int i = floor_lower-1; i < floor_upper; i++) {
        requestsBetween = requestsBetween || (Queue[i].p_orderTypes[ORDER_UP] || Queue[i].p_orderTypes[ORDER_INSIDE] || Queue[i].p_orderTypes[ORDER_DOWN]);
    }
    return requestsBetween;
}



elevator_state queuesystemNewDir() {
    if(!queuesystemRequestBetween(1, HARDWARE_NUMBER_OF_FLOORS)) {
        hardware_command_door_open(0);
        return STATE_IDLE;
    }
    switch (state)
    {
    case STATE_UP_HALT:
        {
        int requestsAbove = queuesystemRequestBetween(current_floor+1, HARDWARE_NUMBER_OF_FLOORS);
        if(requestsAbove) return STATE_UP;
        else return STATE_DOWN;
        break;
        }  
    case STATE_DOWN_HALT: 
        {
        int requestsBelow = queuesystemRequestBetween(1, current_floor-1);
        if(requestsBelow) return STATE_DOWN;
        else return STATE_UP;
        }    
    default:
        return 69;
        break;
    }
}

void queuesystemCheckButtons() {
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        for (int j = 0; j < NUMBER_OF_ORDER_TYPES; j++) {
            if(hardware_read_order(i,j)) {
                request* newRequest = (request*) malloc(sizeof(request));
                newRequest->floor = i+1;
                newRequest->order = j;
                
                queuesystemRequestHandler(newRequest);
                free(newRequest);
            }
        }
    }
}