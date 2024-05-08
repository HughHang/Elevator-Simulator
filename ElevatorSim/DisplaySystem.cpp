#include "DisplaySystem.h"

DisplaySystem::DisplaySystem(string t){
    displayText = t;
}

string DisplaySystem::getDisplayText(){
    return displayText;
}

void DisplaySystem::displayFloor(int f){
    displayText = to_string(f);
}

void DisplaySystem::displayWarning(SignalHandlerType type){
    switch (type){

        case SIG_OVERLOAD:
            displayText = "OVERLOAD";
            break;

        case SIG_FIRE:
            displayText = "FIRE";
            break;

        case SIG_POWER_OUTAGE:
            displayText="POWER OUTAGE";
            break;

        default:
            break;
    }
}

void DisplaySystem::displayInstructions(){
    displayText = "PLEASE DEPART";
}
