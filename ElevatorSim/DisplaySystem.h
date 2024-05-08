#ifndef DISPLAYSYSTEM_H
#define DISPLAYSYSTEM_H

using namespace std;

#include <iostream>
#include <string>
#include "defs.h"

class DisplaySystem{

    public:
        DisplaySystem(string="");
        string getDisplayText();
        void displayFloor(int);
        void displayWarning(SignalHandlerType);
        void displayInstructions();

    private:
        string displayText;

};

#endif // DISPLAYSYSTEM_H
