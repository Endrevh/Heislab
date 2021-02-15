#include "queuesystem.h"
#include "management.h"


void queueRequestHandler(request* p_request) { //hvis vi er i idle, settes direction (state) med en gang i retning etasjen vi fikk request fra
    int requested_floor = p_request->floor;
    orderType order = p_request->order;
    
    switch (state)
    {
    case STATE_IDLE:
        if(requested_floor == current_floor) break;
        else if(requested_floor < current_floor) state = STATE_DOWN;
        else state = STATE_UP;
        Queue[requested_floor-1].p_orderTypes[order] = 1;
        break;
    default:
        Queue[requested_floor-1].p_orderTypes[order] = 1;
        break;
    }
}

void queueDelete(int floor) {
    for(int i = 0; i < NUMBER_OF_ORDER_TYPES; i++) {
            Queue[floor-1].p_orderTypes[i] = 0;
    }
}





