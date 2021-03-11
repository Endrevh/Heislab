#include "controller.h"


#include "stdio.h"


void controllerCheckOrderBtns(void) {
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        for (int j = 0; j < NUMBER_OF_ORDER_TYPES; j++) {
            if(hardware_read_order(i,j)) {
                request* newRequest = (request*) malloc(sizeof(request));
                newRequest->floor = i+1;
                newRequest->order = j;
                
                controllerRequestHandler(newRequest);
                free(newRequest);
            }
        }
    }
}

bool controllerRequestBetween(int floor_lower, int floor_upper) {
    bool requestsBetween = false;
    for(int i = floor_lower-1; i < floor_upper; i++) {
        requestsBetween = requestsBetween || (Queue[i].p_orderTypes[ORDER_UP] || Queue[i].p_orderTypes[ORDER_INSIDE] || Queue[i].p_orderTypes[ORDER_DOWN]);
    }
    return requestsBetween;
}

void controllerRequestHandler(request* p_request) { //hvis vi er i idle, settes direction (state) med en gang i retning etasjen vi fikk request fra
    int requested_floor = p_request->floor;
    orderType order = p_request->order;
    switch (state)
    {
    case STATE_IDLE:
            if(requested_floor == current_floor){
                timerReset(&timer);
                hardware_command_door_open(1);
                for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
                    hardware_command_order_light(requested_floor-1, i, 0);
                }
            }
            else if(requested_floor < current_floor){
                state = STATE_DOWN;
                controllerDepart();
            }
            else{
                state = STATE_UP;
                controllerDepart();
            }
        break;
    case STATE_EMERGENCY_STOP:
        switch(emergency_status)
        {
        case EMERGENCY_HANDLED:
            if(requested_floor == current_floor) {
                state = STATE_IDLE;
                hardware_command_door_open(1);
                timerReset(&timer);
            }
            else if(requested_floor > current_floor) {
                state = STATE_UP;
                hardware_command_door_open(0);
            }
            else if (requested_floor < current_floor){
                state = STATE_DOWN;
                hardware_command_door_open(0);
            }
            break;
        case EMERGENCY_ABOVE: //this means that the elevator is above the last detected floor
            if(requested_floor <= current_floor) {
                state = STATE_DOWN;
                hardware_command_door_open(0);
            }
            else {
                state = STATE_UP;
                hardware_command_door_open(0);
            }
            break;
        case EMERGENCY_BELOW:
            if(requested_floor >= current_floor) {
                state = STATE_UP;
                hardware_command_door_open(0);
            }
            else {
                state = STATE_DOWN;
                hardware_command_door_open(0);
            }
            break;
        }
    default:
        break;
    }
    Queue[requested_floor-1].p_orderTypes[order] = true;
}

void controllerDelFromQueue(int floor) {
    for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
            Queue[floor-1].p_orderTypes[i] = false;
    }
}

elevator_state controllerNewDir(void) {
    if(!controllerRequestBetween(1, HARDWARE_NUMBER_OF_FLOORS)) {
        hardware_command_door_open(0);
        return STATE_IDLE;
    }
    switch (state)
    {
    case STATE_UP_HALT:
        {
        int requestsAbove = controllerRequestBetween(current_floor+1, HARDWARE_NUMBER_OF_FLOORS);
        if(requestsAbove) return STATE_UP;
        else return STATE_DOWN;
        break;
        }  
    case STATE_DOWN_HALT: 
        {
        int requestsBelow = controllerRequestBetween(1, current_floor-1);
        if(requestsBelow) return STATE_DOWN;
        else return STATE_UP;
        }    
    default:
        return 69; //this never happens.
        break;
    }
}


void controllerStopAtFloor(int floor) {
    if(!floor) return;
    bool requestsAbove = controllerRequestBetween(floor+1, HARDWARE_NUMBER_OF_FLOORS);
    bool requestsBelow = controllerRequestBetween(1, floor-1);
    switch (state)
    {
    case STATE_UP:
        if(Queue[floor-1].p_orderTypes[ORDER_UP] == true || Queue[floor-1].p_orderTypes[ORDER_INSIDE] == true || requestsAbove == false) {
            state = STATE_UP_HALT;
            controllerArrived();
        }
        break;
    case STATE_DOWN:
        //sjekker for requests fra etasjer lenger nede
        //stopper hvis det ikke er noen requests under, eller noen skal av eller nedover
        if(Queue[floor-1].p_orderTypes[ORDER_DOWN] == true || Queue[floor-1].p_orderTypes[ORDER_INSIDE] == true || requestsBelow == false) {
            state = STATE_DOWN_HALT;
            controllerArrived();
        }
        break;
    default:
        break;
    }
}



void controllerArrived(void) {
    timerReset(&timer);
    hardware_command_door_open(1);
    
    for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
        hardware_command_order_light(current_floor-1, i, 0);
    }

}

void controllerDepart(void) {
    hardware_command_door_open(0);
    controllerDelFromQueue(current_floor); 
    
    for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
        hardware_command_order_light(current_floor-1, i, 0);
    }
}

//called whenever the emergencybutton is clicked. Checks if the elevator is already handling an emergency,
//and changes elevator state
void controllerEmergencyHandler(void) {
    if(emergency_status == EMERGENCY_HANDLED) {
        switch (state) {
            case STATE_UP:
                emergency_status = EMERGENCY_ABOVE;
                break;
            case STATE_DOWN:
                emergency_status = EMERGENCY_BELOW;
                break;
            case STATE_STARTUP:
                emergency_status = EMERGENCY_BELOW;
            default:
                break;
            }
    }
    state = STATE_EMERGENCY_STOP;
}

int controllerElevatorAtFloor(void) {
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if(hardware_read_floor_sensor(i)) {
            current_floor = i+1;
            emergency_status = EMERGENCY_HANDLED;
            return i+1;
        } 
    }
    return 0;
}