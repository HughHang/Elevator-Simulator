#include "ElevatorControlSystem.h"
#include <QtDebug>

int ElevatorControlSystem::maxElv = MAX_ELEVATORS;
int ElevatorControlSystem::maxFlr = MAX_FLOORS;
int ElevatorControlSystem::doorInterval = DOOR_TIME_INTERVAL;
int ElevatorControlSystem::maxWeight = OVERLOAD_WEIGHT;
int ElevatorControlSystem::safeFloor = SAFE_FLOOR;

ElevatorControlSystem::ElevatorControlSystem(){
    for(int i = 0; i < maxElv; i++){
        elevators.append(new Elevator(1, maxWeight, STA_IDLE, doorInterval));
    }

    for(int j = 0; j < maxFlr; j++){
        floors.append(new Floor(j + 1));
    }

    //Floor direction button pressed
    for(int i = 0; i < maxFlr; i++){
        connect(floors[i]->getButton(FLR_UP),
                SIGNAL(floorDirectionPressed(int, FloorButtonType)), this,
                SLOT(chooseElevator(int, FloorButtonType)));
        connect(floors[i]->getButton(FLR_DOWN),
                SIGNAL(floorDirectionPressed(int, FloorButtonType)), this,
                SLOT(chooseElevator(int, FloorButtonType)));
        connect(floors[i], SIGNAL(addPassengerSignal(tuple<int, int, int>)), this,
                SLOT(addPassengerToList(tuple<int, int, int>)));
    }

}

ElevatorControlSystem::~ElevatorControlSystem(){

    for(int j = 0; j < maxFlr; j++){
        delete floors[j];
    }
    for(int i = 0; i < maxElv; i++){
        delete elevators[i];
    }
}

Elevator* ElevatorControlSystem::getElevator(int num){
    return elevators[num];
}

Floor* ElevatorControlSystem::getFloor(int num){
    return floors[num];
}

int ElevatorControlSystem::getMaxElevators(){
    return maxElv;
}

int ElevatorControlSystem::getMaxFloors(){
    return maxFlr;
}

int ElevatorControlSystem::getCurrentElevatorOfPassenger(int id){
    int elvNum = 0;
    //Find passenger
    for(int i = 0; i < masterList.size(); i++){

        //Passenger found
        if(get<0>(masterList[i]) == id){

            //If not in elevator
            if(get<2>(masterList[i]) == 0){
                    elvNum = 0;
            }
            //If in elevator
            else{
                elvNum = get<2>(masterList[i]);
            }
            break;
        }
    }
    return elvNum;
}

void ElevatorControlSystem::setPassengerElevatorId(int elvId, int passId, int weight){
    //Set the passengers 3rd tuple to the elevator id for identification if they are in an elevator
    get<2>(masterList[masterList.indexOf(make_tuple(passId, weight, 0))]) = elvId;
}

void ElevatorControlSystem::setPassengerFloorDest(int elvNum, int passId, int floorNum){
    //Find the passenger
    for(int i = 0; i < elevators[elvNum - 1]->getPassengers().size(); i++){

        //If passenger is found
        if(elevators[elvNum - 1]->getPassengers()[i]->getID() == passId){
            //Change their dest floor
            elevators[elvNum - 1]->getPassengers()[i]->setDestFloor(floorNum);
            //Add dest floor to elevator list
            elevators[elvNum - 1]->addDestFloor(floorNum);
            //Illuminate elevator button
            elevators[elvNum - 1]->getButton(ELE_FLOOR, floorNum)->illuminate();
        }
    }
}

void ElevatorControlSystem::removePassengers(Elevator*& elv, QList<Passenger*>& elvPass){
    //Loop through passenger list to see if
    //any passengers have destFloor = to current floor
    //If it does then move them out of elevator and free the pointers
    int size = elvPass.size();
    for(int i = 0; i < size; i++){
        if(elvPass[i]->getDestFloor() == elv->getCurrFloor()){
            removePassengerFromList(elvPass[i]->getID(), elvPass[i]->getWeight(), elv->getId());
            elv->leaveElevator(i);
            i--;
            size--;
        }
    }
}

void ElevatorControlSystem::addPassengers(Elevator*& elv, QList<Passenger *>& flrPass){
    //Check if floor list is not empty
    if(!flrPass.isEmpty()){
        //Add all passengers
        for(int i = 0; i < flrPass.size(); i++){

            //Move into elevator
            elv->enterElevator(flrPass[i]);
            setPassengerElevatorId(elv->getId(), flrPass[i]->getID(), flrPass[i]->getWeight());

        }
        //Remove from floor
        flrPass.clear();
    }
}

void ElevatorControlSystem::requestInElevator(int elvNum, SignalHandlerType sig, int passId){

    switch(sig){
        //Help signal
        case SIG_HELP:
            bss.helpPassenger(elvNum, passId);
            break;

        //Close door signal
        case SIG_CLOSE_DOOR:
            //If elevator door is open
            if(elevators[elvNum - 1]->getState() == STA_OPEN){
                qDebug() << "Passenger #" << passId << "pressed close door button";
                //Close
                elevators[elvNum - 1]->closeDoors();
            }
            //If elevator door is not open
            else{
                qDebug() << "Elevator #" << elvNum << "is not currently open";
            }
            break;

        //Open door signal
        case SIG_OPEN_DOOR:
            //If door is open and timer is less than door open duration
            if(elevators[elvNum - 1]->getTimer() < doorInterval &&
                    elevators[elvNum - 1]->getState() == STA_OPEN){
                qDebug() << "Passenger #" << passId << "pressed open door button";
                elevators[elvNum - 1]->setTimer(doorInterval);
            }
            //Door isnt open
            else{
                qDebug() << "Elevator #" << elvNum <<
                            "open door button not currently available";
            }
            break;

        //Light sensor signal
        case SIG_OBSTACLE:
            //If someone disrupts light sensor
            qDebug() << "Passenger #" << passId << "disrupted light sensor";
            elevators[elvNum - 1]->setState(STA_OBSTACLE);
            break;

        default:
            break;
    }
}

void ElevatorControlSystem::emergency(SignalHandlerType type){
    switch(type){

        case SIG_FIRE:
            qInfo("\n* EMERGENCY FIRE *\n");
            break;

        case SIG_POWER_OUTAGE:
            qInfo("\n* EMERGENCY POWER OUTAGE *");
            break;

        default:
            break;

    }
    for(int i = 0; i < maxElv; i++){
        elevators[i]->goToEmergencyFloor(safeFloor, type);

    }
}

void ElevatorControlSystem::dimButtons(Elevator*& elv, int currFloor){

    //Check floor button first
    floors[currFloor - 1]->getButton(FLR_UP)->dimOut();
    floors[currFloor - 1]->getButton(FLR_DOWN)->dimOut();

    //Check elevator button
    for(int i = 0; i < elv->getButtons().size(); i++){
        elv->getButtons()[i]->dimOut();
    }
}

void ElevatorControlSystem::removePassengerFromList(int id, int weight, int elvId){
    //Remove passenger from master list
    masterList.removeOne(make_tuple(id, weight, elvId));

    emit removePassengerSignal(id);
}


void ElevatorControlSystem::chooseElevator(int floorReq, FloorButtonType dir){
    qInfo("\n*Choosing Elevator*");
    Elevator* elevator = NULL;

    //Get direction to compare directions between floor type and elevator current direction
    ElevatorStateType stateDir = STA_STOP;
    if(dir == FLR_UP){
        stateDir = STA_MOVING_UP;
    }
    else{
        stateDir = STA_MOVING_DOWN;
    }

    //Max difference
    int currDiff = maxFlr;
    //Check each elevator
    for(int i = 0; i < maxElv; i++){

        //Check if the elevator is idle or if it's moving in the direction of the requested floor
        if(elevators[i]->getState() == STA_IDLE ||
                elevators[i]->getState() == STA_STOP ||
                elevators[i]->getState() == stateDir){

            //Find distance between floors
            int newDiff = floorReq - elevators[i]->getCurrFloor();

            //If it's going up check if it's over
            //If it's going down check if it's under
            if((stateDir == STA_MOVING_UP && newDiff >= 0) ||
                    (stateDir == STA_MOVING_DOWN && newDiff <= 0) ||
                    elevators[i]->getState() == STA_IDLE ||
                    elevators[i]->getState() == STA_STOP){

                //If the difference is smaller then we can choose this elevator
                //Keep value in case next elevator is closer
                if(abs(newDiff) <= currDiff){
                    currDiff = abs(newDiff);
                    elevator = elevators[i];
                }

            }
        }
    }

    //Add destination floor to the destination list for the elevator
    if(elevator != NULL){

        qDebug() << "Closest elevator is Elevator #" << elevator->getId() << "\n";

        elevator->addDestFloor(floorReq);

        //Set the direction of the elevator
        //If elevator needs to go up
        if(floorReq > elevator->getCurrFloor()){
            elevator->setState(STA_MOVING_UP);
        }
        //If elevator needs to go down
        else if (floorReq < elevator->getCurrFloor()){
            elevator->setState(STA_MOVING_DOWN);
        }
        else{
            elevator->setState(STA_STOP);
        }

    }else{

        qInfo("No elevator available currently \n");

    }
}

void ElevatorControlSystem::moveElevatorsToFloor(){
    //Loop through each elevator
    for(int i = 0; i < maxElv; i++){

        //Check if elevator has any requests
        if(elevators[i]->checkRequests()){

            ElevatorStateType currState = elevators[i]->getState();

            //Check if elevator is available to move or already at the floor
            if(currState == STA_MOVING_UP ||
                    currState == STA_MOVING_DOWN ||
                    currState == STA_STOP ||
                    currState == STA_EMERGENCY){

                //Move the elevator to next floor
                elevators[i]->moveToNextFloor();
            }
        }
    }
}

void ElevatorControlSystem::checkElevators(){
    for(int i = 0; i < maxElv; i++){
        //Check if reached a floor destination
        if(elevators[i]->reachDestFloor()){
            if(elevators[i]->getState() == STA_OVERLOAD){
                //Check overload
                if(elevators[i]->checkOverload()){
                    //Get last passenger to join the elevator
                    Passenger* lastPassenger = NULL;
                    lastPassenger = elevators[i]->getPassengers()[elevators[i]->getPassengers().size() - 1];

                    //Remove from master list
                    removePassengerFromList(
                        lastPassenger->getID(),
                        lastPassenger->getWeight(),
                        elevators[i]->getId());

                    //Make them leave elevator
                    elevators[i]->leaveElevator(elevators[i]->getPassengers().size() - 1);
                }
            }
            //Fire or power outage
            else if(elevators[i]->getState() == STA_EMERGENCY){
                //Open doors
                elevators[i]->openDoors();
                removePassengers(elevators[i], elevators[i]->getPassengers());
                elevators[i]->giveInstructions();
            }
            //If not overloaded do normal operation
            else if(elevators[i]->getState() == STA_STOP ||
                    elevators[i]->getState() == STA_MOVING_UP ||
                    elevators[i]->getState() == STA_MOVING_DOWN){
                //Check if button is still illuminated
                dimButtons(elevators[i], elevators[i]->getCurrFloor());

                //Open doors
                elevators[i]->openDoors();

                //Check if passengers in elevator need to leave
                removePassengers(elevators[i], elevators[i]->getPassengers());

                //Check if passengers in floor need to come in
                addPassengers(elevators[i],
                              floors[elevators[i]->getCurrFloor() - 1]->getPassengers());

                //Check overload
                elevators[i]->checkOverload();
            }
            //If timer hits 0 start to close doors
            else if(elevators[i]->getTimer() == 0){
                elevators[i]->closeDoors();
                elevators[i]->setTimer(elevators[i]->getTimer() - 1);
            }
            //If the door is open
            else if(elevators[i]->getState() == STA_OPEN){
                //Reduce timer
                elevators[i]->setTimer(elevators[i]->getTimer() - 1);
            }
            //While closing
            //If someone interrupts the light sensor
            else if(elevators[i]->getState() == STA_OBSTACLE){
                //Open the door and reset timer
                elevators[i]->openDoors();
                elevators[i]->setTimer(doorInterval);
            }
            //If no one interrupts the light sensor
            else if (elevators[i]->getState() == STA_CLOSING){
                //Close doors
                elevators[i]->setState(STA_CLOSE);
                qInfo("*DOORS CLOSE*");
            }
            //If the doors are finally close
            else if(elevators[i]->getState() == STA_CLOSE){
                //if there are no people in elevator and no requests, go to idle
                if(!elevators[i]->checkRequests() && elevators[i]->getPassengers().isEmpty()){
                    elevators[i]->setState(STA_IDLE);
                }
                //if there are people in the elevator go to stop
                else if(!elevators[i]->getPassengers().isEmpty()){
                    elevators[i]->setState(STA_STOP);
                }
                //Reset timer and current floor from destination list
                elevators[i]->setTimer(doorInterval);
                elevators[i]->removeDestFloor();
            }
        }
    }
}


void ElevatorControlSystem::addPassengerToList(tuple<int, int, int> p){
    //Add passenger to master list
    masterList.append(p);

}




