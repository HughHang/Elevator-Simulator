#ifndef DEFS_H
#define DEFS_H

typedef enum {ELE_FLOOR, ELE_DOOR_CONTROL, ELE_HELP} ElevatorButtonType;
typedef enum {FLR_UP, FLR_DOWN} FloorButtonType;
typedef enum {SIG_HELP, SIG_OBSTACLE, SIG_FIRE, SIG_OVERLOAD, SIG_POWER_OUTAGE,
              SIG_OPEN_DOOR, SIG_CLOSE_DOOR} SignalHandlerType;
typedef enum {STA_IDLE, STA_MOVING_UP, STA_MOVING_DOWN, STA_STOP,
              STA_OPEN, STA_CLOSING, STA_CLOSE, STA_READY,
              STA_EMERGENCY, STA_OVERLOAD, STA_OBSTACLE} ElevatorStateType;
typedef enum {UP, DOWN, NONE} Direction;

//Starting elevator IDs
#define ELEVATOR_IDS 1;
#define PASSENGER_ID 1;

//Max floors and elevators;
#define MAX_FLOORS 7;
#define MAX_ELEVATORS 3;

//Safe floor number for elevators to go to during emergency
#define SAFE_FLOOR 1;
#define OVERLOAD_WEIGHT 500;

//Time to takes to update
#define TIME_INTERVAL 1000;
//How long the door needs to stay open
#define DOOR_TIME_INTERVAL 10;

#endif // DEFS_H
