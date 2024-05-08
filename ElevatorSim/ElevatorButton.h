#ifndef ELEVATORBUTTON_H
#define ELEVATORBUTTON_H

using namespace std;

#include <iostream>
#include <string>

#include "Button.h"
#include "defs.h"
#include <QDebug>
#include "QtGlobal"

class ElevatorButton : public Button{
    Q_OBJECT

    public:
        ElevatorButton(
            bool=false,
            int=0,
            ElevatorButtonType=ELE_FLOOR,
            int=1
        );
        ElevatorButtonType getType();
        virtual void sendSignal();

    private:
        ElevatorButtonType type;
        int buttonValue;

};

#endif // ELEVATORBUTTON_H
