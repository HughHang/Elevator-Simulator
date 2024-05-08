#ifndef PASSENGER_H
#define PASSENGER_H

using namespace std;

#include <iostream>
#include <string>
#include <QObject>
#include <QDebug>

#include "Button.h"
#include "defs.h"

class Passenger : public QObject{
    Q_OBJECT

    public:
        Passenger(int=100);

        int getID();
        float getWeight();
        int getDestFloor();

        void setDestFloor(int);
        void setSafeFloor(int);

    signals:
        void passengerChooseFloor(int);


    private:
        static int nextId;

        int id;
        int weight;
        int destFloor;

};

#endif // PASSENGER_H
