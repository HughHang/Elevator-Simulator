#include "FloorButton.h"

FloorButton::FloorButton(bool p, int n, FloorButtonType dir)
    : Button(p, n), direction(dir){
}

FloorButtonType FloorButton::getDirection(){
    return direction;
}

void FloorButton::sendSignal(){
    illuminate();
    switch(direction){

        //Up request
        case 0:
            //qDebug() << "Passenger #"<< passId << "has requested to go up at floor" << id;
            qDebug() << "Passenger has requested to go up at floor" << id;
            emit floorDirectionPressed(id, direction);
            break;

        //Down request
        case 1:
            //qDebug() << "Passenger #"<< passId << "has requested to go down at floor" << id;
            qDebug() << "Passenger has requested to go down at floor" << id;
            emit floorDirectionPressed(id, direction);
            break;
    }
}
