#ifndef MANAGEMENT_H
#define MANAGEMENT_H

typedef enum {
    STATE_STARTUP = 1,
    STATE_UP,
    STATE_UP_HALT,
    STATE_DOWN,
    STATE_DOWN_HALT,
    STATE_IDLE
} elevator_state;

elevator_state state;
int current_floor;

void managementArrived(int floor); //slukke lys (se main.c) og slette fra kø

void managementDepart(void);

int managementElevatorAtFloor(void); //sjekker om heisen er ved en etasje. 

void managementStopAtFloor(int floor);  //sjekker om heisen skal stoppe ved gitt etasje. Må se om noen skal av, eller om noen skal på i den retningen heisen er på vei (bruk state til å sjekke dette)
                                        //kalles når vi går forbi en etasje                                       
                                        //bytte state
                                        //må aldri gå lenger ned enn nederste request, og motsatt. Dette kunne skjedd dersom man er i idle,
                                        //får en request lenger nede som skal opp. Da vil heisen begynne å gå nedover selv om det er ingen lenger nede som skal ned eller av.
                                        //foreslått løsning: sjekke om det er noen i etasjene under som skal av eller oppover. Hvis ja, fortsett nedover. Sett opp et eksempel med dette, se hva som fungerer. Kanskje ha en bool variabel som settes til true/false når første idle-request er fullført


#endif