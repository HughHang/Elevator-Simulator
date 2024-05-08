#ifndef ELEVATORCONTROLSYSTEM_H
#define ELEVATORCONTROLSYSTEM_H

using namespace std;

#include <iostream>
#include <string>
#include <QObject>
#include <QDebug>

#include "QtGlobal"
#include "Elevator.h"
#include "Floor.h"
#include "BuildingSafetyServices.h"
#include "defs.h"

class ElevatorControlSystem : public QObject {
    Q_OBJECT

    public:
        ElevatorControlSystem();
        ~ElevatorControlSystem();

        //Geters
        Elevator* getElevator(int);
        Floor* getFloor(int);
        int getMaxElevators();
        int getMaxFloors();
        int getCurrentElevatorOfPassenger(int);

        //Setters
        void setPassengerElevatorId(int, int, int);
        void setPassengerFloorDest(int, int, int);

        void removePassengers(Elevator*&, QList<Passenger*>&);
        void addPassengers(Elevator*&, QList<Passenger*>&);
        void requestInElevator(int, SignalHandlerType, int);
        void emergency(SignalHandlerType);
        void dimButtons(Elevator*&, int);
        void removePassengerFromList(int, int, int);

    public slots:
        void chooseElevator(int, FloorButtonType);
        void moveElevatorsToFloor();
        void checkElevators();
        void addPassengerToList(tuple<int, int, int>);


    signals:
        void removePassengerSignal(int);


    private:
        static int maxElv;
        static int maxFlr;
        static int doorInterval;
        static int maxWeight;
        static int safeFloor;

        BuildingSafetyServices bss;

        QList<Floor*> floors;
        QList<Elevator*> elevators;
        QList<tuple<int, int, int>> masterList;

};

#endif // ELEVATORCONTROLSYSTEM_H
