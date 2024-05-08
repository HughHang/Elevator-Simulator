#include "ElevatorButton.h"

ElevatorButton::ElevatorButton(bool p, int n, ElevatorButtonType t, int v)
    : Button(p, n), type(t), buttonValue(v){}

void ElevatorButton::sendSignal(){
    illuminate();

    switch(type){
        //Floor type button
        case 0:
            qDebug() << "Floor" << buttonValue << "requested in elevator" << id;
            break;

        //Door control type button
        case 1:
            if (buttonValue == 0){
                qDebug() << "Door open requested for elevator" << id;
                emit
            }
            else{
                qDebug() << "Door close requested for elevator" << id;
            }
            break;

        //Help type button
        case 2:
            qDebug() << "Help requested in Elevator #" << id;
            break;
    }
}
