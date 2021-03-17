#include "timer.h"




bool timerTrigger(Timer* timer_p, int trigger) {
    return((clock() - timer_p->timestamp)/CLOCKS_PER_SEC >= trigger);
}


void timerReset(Timer* timer_p) {
    timer_p->timestamp = clock();
}


