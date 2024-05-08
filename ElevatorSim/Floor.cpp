#include "Floor.h"
#include "defs.h"

Floor::Floor(int n){
    int size = MAX_FLOORS;
    if(n < size + 1){
        floorNum = n;
        addButtons();
    }
}

Floor::~Floor(){
    for(int i = 0; i < 2; i++){
        delete buttons[i];
    }
}

void Floor::addButtons(){
    buttons.append(new FloorButton(false, floorNum, FLR_UP));
    buttons.append(new FloorButton(false, floorNum, FLR_DOWN));
}

Button* Floor::getButton(FloorButtonType type){

    //Request to go up
    if(type == FLR_UP){

        return buttons[0];

    }

    //Request to go down
    return buttons[1];

}

QList<Passenger*>& Floor::getPassengers(){
    return passengers;
}

void Floor::createPassenger(){

    passengers.append(new Passenger(100));

    //Add passenger to master list
    tuple <int, int, int> passenger;
    passenger = make_tuple(passengers[passengers.size()- 1]->getID(),
            passengers[passengers.size()- 1]->getWeight(), 0);

    //Send signal to create add to master list and combo box
    emit addPassengerSignal(passenger);

    qDebug() << "New Passenger #" << passengers[passengers.size()- 1]->getID();

}
