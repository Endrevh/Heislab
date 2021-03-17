/**
 * @file
 * @brief Queuesystem- and elevator control related logic.
 *
 * Lars Rønhaug Pettersen, Endre Vee Hagestuen
 */

#pragma once
#include "hardware.h"
#include "timer.h"
#include "stdlib.h"
#include <stdbool.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H
#define NUMBER_OF_ORDER_TYPES 3
#define FLOOR_WAIT_TIME 3

/**
 * @brief Types of orders
 */

typedef enum {
    ORDER_UP = 0,
    ORDER_INSIDE,
    ORDER_DOWN
} orderType;

/**
 * @brief contains commands from each floor
 * Contains three bools representing the order types from @c orderType
 */
typedef struct
{
   bool p_orderTypes[NUMBER_OF_ORDER_TYPES];
} command;

/**
 * @brief represents an elevator-request containing floor number and order type
 * 
 */
typedef struct
{
   int floor;
   orderType order;
}request;

/**
 * @brief contains the elevator states
 */
typedef enum {
    STATE_STARTUP = 1,
    STATE_UP,
    STATE_UP_HALT,
    STATE_DOWN,
    STATE_DOWN_HALT,
    STATE_IDLE,
    STATE_EMERGENCY_STOP
} elevator_state;

/**
 * @brief contains the emergency types
 * 
 * If the elevator is not in an emergency, the status will be @c EMERGENCY_HANDLED
 */
typedef enum {
    EMERGENCY_HANDLED,
    EMERGENCY_ABOVE,
    EMERGENCY_BELOW
} emergency_types;

elevator_state state;
emergency_types emergency_status;
int current_floor;

command Queue[HARDWARE_NUMBER_OF_FLOORS];

/**
 * @brief polls order buttons and creates associated requests
 * 
 * If an order is found and a request is created, the request is passed to @c controllerRequestHandler
 */
void controllerCheckOrderBtns(void);

/**
 * @brief checks for requests between given floors
 * 
 * @param floor_lower lowest floor checked
 * @param floor_upper highest floor checked
 * 
 * @return returns true if any request is found, returns false otherwise
 */
bool controllerRequestBetween(int floor_lower, int floor_upper);

/**
 * @brief adds request to queue and also changes elevator state if in @c STATE_IDLE
 * or @c STATE_EMERGENCY_STOP
 * 
 * @param p_request pointer to request of type @c request
 */
void controllerRequestHandler(request* p_request);

/**
 * @brief deletes all orders from given floor
 * 
 * @param floor floor to delete orders from
 */
void controllerDelFromQueue(int floor);

/**
 * @brief calculates new elevator state based on queue, current floor and current state.
 * This function is called after @c FLOOR_WAIT_TIME seconds stop to allow new passengers to
 * first choose desired floor.
 * 
 * @return returns elevator state. Possible return values are @c STATE_IDLE, @c STATE_UP and @c STATE_DOWN
 */
elevator_state controllerNewDir(void);

/**
 * @brief decides if the elevator stops at passing floor and if so,
 * changes state to @c STATE_UP_HALT or @c STATE_DOWN_HALT
 * 
 * @param floor equals the passing floor, 0 if undefined floor
 */
void controllerStopAtFloor(int floor);                                                                               
                                                                                

/**
 * @brief Functionality when elevator arrives at floor.
 * This will reset timer, open door and turn off order lights
 */
void controllerArrived(void); 

/**
 * @brief Functionality when elevator departs from floor
 * Closes door, deletes orders from departing floor. Also turns off order lights again at @c current_floor
 * in case of new orders while at floor
 */
void controllerDepart(void);

/**
 * @brief handles emergency-related logic. 
 * This is implemented to prevent disorientation when in an emergency
 */
void controllerEmergencyHandler(void);

/**
 * @brief Checks if the elevator is at a floor
 * @return floor number if success, 0 if no floor
*/
int controllerElevatorAtFloor(void);

#endif

