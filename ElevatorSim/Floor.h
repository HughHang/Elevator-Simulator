#ifndef FLOOR_H
#define FLOOR_H

using namespace std;

#include <iostream>
#include <string>
#include <QObject>

#include "FloorButton.h"
#include "Passenger.h"
#include "defs.h"

class Floor : public QObject{
    Q_OBJECT

    public:
        Floor(int=1);
        ~Floor();
        void addButtons();
        Button* getButton(FloorButtonType);
        QList<Passenger*>& getPassengers();

    public slots:
        void createPassenger();

    signals:
        void addPassengerSignal(tuple<int, int, int>);

    private:
        int floorNum;
        QList<FloorButton*> buttons;
        QList<Passenger*> passengers;

};

#endif // FLOOR_H
