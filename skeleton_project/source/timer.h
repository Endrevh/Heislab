#pragma once
#include "time.h"
#include <stdbool.h>


/**
 * @brief Timer typedef. Contains timestamp used for comparing
 */
typedef struct
{
   clock_t timestamp;
} Timer;

Timer timer;

/**
 * @brief checks if timer has elapsed more than @p trigger seconds
 * 
 * @param timer_p pointer to timer
 * @param trigger timer overflow threshold
 * 
 * @return returns true if timer has overflowed, false otherwise
 */
bool timerTrigger(Timer* timer_p, int trigger);

/**
 * @brief resets timer by setting @c timestamp to current time
 * 
 * @param timer we want to reset
 */
void timerReset(Timer* timer_p);





