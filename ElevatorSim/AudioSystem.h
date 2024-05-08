#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

using namespace std;

#include <iostream>
#include <string>
#include <QDebug>

#include "QtGlobal"

class AudioSystem{

    public:
        AudioSystem();
        void ringBell(int);
        void soundAlarm(int);

};

#endif // AUDIOSYSTEM_H
