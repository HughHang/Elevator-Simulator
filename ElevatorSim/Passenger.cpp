#include "Passenger.h"

int Passenger::nextId = PASSENGER_ID;

Passenger::Passenger(int w){
    id = nextId++;
    weight = w;
    destFloor = 0;

}

int Passenger::getID(){
    return id;
}

float Passenger::getWeight(){
    return weight;
}

int Passenger::getDestFloor(){
    return destFloor;
}

void Passenger::setDestFloor(int f){
    qDebug() << "Passenger" << id << "requested to go to floor" << f;
    destFloor = f;
}

void Passenger::setSafeFloor(int f){
    destFloor = f;
}
