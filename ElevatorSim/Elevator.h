#ifndef ELEVATOR_H
#define ELEVATOR_H

using namespace std;

#include <iostream>
#include <string>
#include <QObject>
#include <QDebug>
#include <algorithm>

#include "QtGlobal"

#include "AudioSystem.h"
#include "DisplaySystem.h"
#include "Passenger.h"
#include "ElevatorButton.h"
#include "defs.h"
class Elevator{

    public:
        Elevator(
            int=1,
            int=300,
            ElevatorStateType=STA_IDLE,
            int=10
        );
        ~Elevator();
        //Getters
        int getId();
        int getCurrFloor();
        int getMaxWeight();
        int getCurrentWeight();
        ElevatorStateType getState();
        ElevatorButton* getButton(ElevatorButtonType, int);
        QList<ElevatorButton*>& getButtons();
        QList<Passenger*>& getPassengers();
        string getDisplayText();
        int getTimer();

        //Setters
        void setState(ElevatorStateType);
        void setTimer(int);

        void addDestFloor(int);
        void moveToNextFloor();
        bool reachDestFloor();
        void removeDestFloor();
        bool checkRequests();
        void stopMoving();
        void openDoors();
        void closeDoors();
        void leaveElevator(int);
        void enterElevator(Passenger*);
        bool checkOverload();
        void goToEmergencyFloor(int, SignalHandlerType);
        void giveInstructions();

    public slots:
        void setDestFloorOfPassenger(int, int);

    private:
        static int nextId;

        int id;
        int currFloor;
        int maxWeight;
        Direction direction;
        ElevatorStateType state;
        int currWeight = 0;
        int timer;

        AudioSystem audioSystem;
        DisplaySystem displaySystem;

        QList<Passenger*> passengers;
        QList<ElevatorButton*> buttons;
        QList<int> destFloors;

};

#endif // ELEVATOR_H
