#include "timer.h"



clock_t timestamp = 0;


bool timerTrigger(int trigger) {
    return((clock() - timestamp)/CLOCKS_PER_SEC >= trigger);
}


void timerReset(void) {
    timestamp = clock();
}


