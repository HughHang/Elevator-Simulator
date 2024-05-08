#ifndef PASSENGERLIST_H
#define PASSENGERLIST_H

#include <QObject>

#include "Passenger.h"
#include "QtGlobal"

class PassengerList : public QObject{
    Q_OBJECT


public:
    PassengerList();
    Passenger getPassenger(int);
    void addPassenger(Passenger*);
    void removePassenger(int);
    QList<Passenger*>& getAllPassengers();

private:
    QList<Passenger*> allPassengers;
};

#endif // PASSENGERLIST_H
