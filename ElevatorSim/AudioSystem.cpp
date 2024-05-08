#include "AudioSystem.h"

AudioSystem::AudioSystem(){}

void AudioSystem::ringBell(int id){
    qDebug() << "Elevator" << id << "- *Rings bell*";
}

void AudioSystem::soundAlarm(int id){
    qDebug() << "Elevator" << id << "- *WEEWOOWEEWOO*";
}
