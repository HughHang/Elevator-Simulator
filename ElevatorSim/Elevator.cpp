#include "Elevator.h"

int Elevator::nextId = ELEVATOR_IDS;

Elevator::Elevator(int curr, int max, ElevatorStateType s, int t){
    id = nextId++;
    currFloor = curr;
    maxWeight = max;
    state = s;
    timer = t;
    direction = NONE;
    buttons.append(new ElevatorButton(false, id, ELE_HELP, 0));
    buttons.append(new ElevatorButton(false, id, ELE_DOOR_CONTROL, 0));
    buttons.append(new ElevatorButton(false, id, ELE_DOOR_CONTROL, 1));
    int size = MAX_FLOORS;
    for(int i = 0; i < size + 1; i++){
        buttons.append(new ElevatorButton(false, id, ELE_FLOOR, i + 1));
    }
    displaySystem.displayFloor(currFloor);
}

Elevator::~Elevator(){


    //Delete help, open, and close buttons, plus floor buttons
    int size = buttons.size();
    for(int i = 0; i < size; i++){
        delete buttons[i];
    }
    //Delete passengers
    size = passengers.size();
    for(int j = 0; j < size; j++){
        delete passengers[j];
    }

}

int Elevator::getId(){
    return id;
}

int Elevator::getCurrFloor(){
    return currFloor;
}

int Elevator::getMaxWeight(){
    return maxWeight;
}

int Elevator::getCurrentWeight(){
    return currWeight;
}

ElevatorStateType Elevator::getState(){
    return state;
}

ElevatorButton* Elevator::getButton(ElevatorButtonType type, int value){
    switch(type){
        case ELE_HELP:
            return buttons[0];
            break;
        case ELE_DOOR_CONTROL:
            if(value == 0){
                return buttons[1];
            }
            else{
                return buttons[2];
            }
            break;
        case ELE_FLOOR:
            return buttons[value + 2];
            break;
    }
    return buttons[0];
}

QList<ElevatorButton*>& Elevator::getButtons(){
    return buttons;
}

QList<Passenger*>& Elevator::getPassengers(){
    return passengers;
}

string Elevator::getDisplayText(){
    return displaySystem.getDisplayText();
}

int Elevator::getTimer(){
    return timer;
}

void Elevator::setState(ElevatorStateType newState){
    state = newState;
}

void Elevator::setTimer(int t){
    timer = t;
}

void Elevator::addDestFloor(int floorNum){

    //Add floor to list
    destFloors.append(floorNum);
    //Sort the list
    sort(destFloors.begin(), destFloors.end());

    //If going up set direction to up
    if(floorNum > currFloor){
        direction = UP;
    }
    //If doing down reverse list and set direction to down
    else{
        direction = DOWN;
        std::reverse(destFloors.begin(), destFloors.end());
    }
}

void Elevator::moveToNextFloor(){

    //If not at destination floor
    if(destFloors[0] != currFloor){
    //If higher than current floor, change state to moving up and move elevator up
        if(destFloors[0] > currFloor){
            if(state != STA_EMERGENCY){
                state = STA_MOVING_UP;
            }
            direction = UP;
            currFloor++;
        }
        //if lower than current floor, change state to moving down and move elevator down
        else{
            if(state != STA_EMERGENCY){
                state = STA_MOVING_DOWN;
            }
            direction = DOWN;
            currFloor--;
        }
    }
    qDebug() << "Elevator #" << id << "has reached floor" << currFloor;
    if(destFloors[0] == currFloor){
        if(state != STA_EMERGENCY){
            state = STA_STOP;
        }
        qInfo("* Floor reached *\n");
    }

    //Set display text
    if(state != STA_EMERGENCY){
        displaySystem.displayFloor(currFloor);
    }
}

bool Elevator::reachDestFloor(){
    if(destFloors.isEmpty()){
        direction = NONE;
        return false;
    }
    else{
        if(currFloor == destFloors[0]){
            return true;
        }
        return false;
    }
}

void Elevator::removeDestFloor(){
    if(currFloor == destFloors[0]){
        destFloors.removeFirst();
    }
}

bool Elevator::checkRequests(){
    if(destFloors.empty() || destFloors[0] == currFloor){
        return false;
    }
    return true;
}

void Elevator::stopMoving(){
    qDebug() << "Elevator #" << id << "has stopped on floor #" << currFloor;
    state = STA_STOP;
}

void Elevator::openDoors(){
    audioSystem.ringBell(id);
    qDebug() << "Elevator #" << id << " - *DOORS OPEN*";
    state = STA_OPEN;
}

void Elevator::closeDoors(){
    qDebug() << "Elevator #" << id << " - *DOORS CLOSING*\n";
    state = STA_CLOSING;
}

void Elevator::leaveElevator(int p){
    qDebug() << "Passenger #" << passengers[p]->getID() << "has left Elevator #" << id;
    Passenger* temp = NULL;
    temp = passengers[p];
    currWeight = currWeight - temp->getWeight();
    passengers.removeAt(p);
    delete temp;
}

void Elevator::enterElevator(Passenger * p){
    passengers.append(p);
    qDebug() << "Passenger #" << p->getID() << "has entered the Elevator #" << id;
    currWeight = currWeight + p->getWeight();
}

bool Elevator::checkOverload(){
    //If weight is greater than overload weight
    if(currWeight > maxWeight){

        qInfo("* ELEVATOR OVERLOAD *");
        displaySystem.displayWarning(SIG_OVERLOAD);
        audioSystem.soundAlarm(id);
        state = STA_OVERLOAD;
        return true;

    }
    displaySystem.displayFloor(currFloor);
    state = STA_OPEN;
    return false;

}

void Elevator::goToEmergencyFloor(int safeFloor, SignalHandlerType type){

    audioSystem.soundAlarm(id);
    //Clear any destinations
    destFloors.clear();

    //Add safeFloor to destination list
    destFloors.append(safeFloor);

    //Set state to emergency
    state = STA_EMERGENCY;

    //Change all destinations for passengers to safe floor
    for(int i = 0; i < passengers.size(); i++){
        passengers[0]->setSafeFloor(safeFloor);
    }

    //Change display
    displaySystem.displayWarning(type);
}

void Elevator::giveInstructions(){
    displaySystem.displayInstructions();
    audioSystem.soundAlarm(id);
}
