#include "timer.h"


time_t timestamp = 0;


int timerTrigger(int trigger) {
    return(time(NULL) - timestamp >= (long)trigger);
}

void timerReset(void) {
    timestamp = time(NULL);
}

