#include "time.h"


time_t timestamp;

//returns true if timer has elapsed more than {trigger} seconds
int timerTrigger(int trigger);

//updates timestamp to current time
void timerReset(void);


//consider having a logic for activating/disabling the timer. Make a struct?


