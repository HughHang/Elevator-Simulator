#ifndef BUTTON_H
#define BUTTON_H

using namespace std;

#include <iostream>
#include <string>
#include <QObject>

#include "QtGlobal"
#include "defs.h"

//#include "ElevatorControlSystem.h"

class Button : public QObject{
    Q_OBJECT

    public:
        Button(bool=false, int=0);
        void illuminate();
        void dimOut();
        bool getPressed();

    public slots:
        virtual void sendSignal() = 0;

    private:
        bool pressed;

    protected:
        int id;
};

#endif // BUTTON_H
