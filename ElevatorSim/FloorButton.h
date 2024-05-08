#ifndef FLOORBUTTON_H
#define FLOORBUTTON_H

using namespace std;

#include <iostream>
#include <string>
#include <QDebug>

#include "QtGlobal"

#include "Button.h"
#include "defs.h"

class FloorButton : public Button{
    Q_OBJECT

    public:
        FloorButton(bool=false, int=1, FloorButtonType = FLR_UP);
        FloorButtonType getDirection();
        virtual void sendSignal();

    signals:
        void floorDirectionPressed(int, FloorButtonType);

    private:
        FloorButtonType direction;

};

#endif // FLOORBUTTON_H
