#include "timer.h"



time_t timestamp = 0;


bool timerTrigger(int trigger) {
    return(time(NULL) - timestamp >= (long)trigger);
}

void timerReset(void) {
    timestamp = time(NULL);
}


