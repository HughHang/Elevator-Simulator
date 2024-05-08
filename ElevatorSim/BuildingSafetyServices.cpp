#include "BuildingSafetyServices.h"

BuildingSafetyServices::BuildingSafetyServices(){}

void BuildingSafetyServices::helpPassenger(int elvId, int passId){
    qDebug() << "* Helping Passenger #" << passId << "in Elevator #" << elvId << "*";
}
