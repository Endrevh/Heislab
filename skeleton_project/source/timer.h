#include "time.h"
#include <stdbool.h>


time_t timestamp;

//returns true if timer has elapsed more than {trigger} seconds
bool timerTrigger(int trigger);

//updates timestamp to current time
void timerReset(void);


//consider having a logic for activating/disabling the timer. Make a struct?


