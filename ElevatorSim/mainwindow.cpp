#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::interval = TIME_INTERVAL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ecs = new ElevatorControlSystem();

    //Timer
    timeInterval = new QTimer(this);
    timeInterval->start(interval);

    //Check for elevators if they need to move
    connect(timeInterval, SIGNAL(timeout()), ecs, SLOT(moveElevatorsToFloor()));
    connect(timeInterval, SIGNAL(timeout()), ecs, SLOT(checkElevators()));
    connect(timeInterval, SIGNAL(timeout()), this, SLOT(updateDisplay()));
    connect(timeInterval, SIGNAL(timeout()), this, SLOT(updateElevator()));

    //For adding passengers to combo box
    for(int i = 0; i < ecs->getMaxFloors(); i++){
        connect(ecs->getFloor(i), SIGNAL(addPassengerSignal(tuple<int, int, int>)), this,
                SLOT(addPassengerToComboBox(tuple<int, int, int>)));
    }
    //For removing passengers from combo box
    connect(ecs, SIGNAL(removePassengerSignal(int)), this,
            SLOT(removePassengerFromComboBox(int)));

    //Add elevators to combo box
    for(int j = 0; j < ecs->getMaxElevators(); j++){
        ui->cb_elevatorNum->addItem(QString("%3").arg(j + 1));
    }

    //Elevator Buttons//
    //Help, Door open, Door close, Floor Buttons
    connect(ui->btn_floor1, SIGNAL(released()), this, SLOT(floor1Request()));
    connect(ui->btn_floor2, SIGNAL(released()), this, SLOT(floor2Request()));
    connect(ui->btn_floor3, SIGNAL(released()), this, SLOT(floor3Request()));
    connect(ui->btn_floor4, SIGNAL(released()), this, SLOT(floor4Request()));
    connect(ui->btn_floor5, SIGNAL(released()), this, SLOT(floor5Request()));
    connect(ui->btn_floor6, SIGNAL(released()), this, SLOT(floor6Request()));
    connect(ui->btn_floor7, SIGNAL(released()), this, SLOT(floor7Request()));
    connect(ui->btn_help, SIGNAL(released()), this, SLOT(helpRequest()));
    connect(ui->btn_close, SIGNAL(released()), this, SLOT(closeDoorRequest()));
    connect(ui->btn_open, SIGNAL(released()), this, SLOT(openDoorRequest()));

    //Safety Handlers//
    //Light sensor, fire sensor, power outage sensor
    connect(ui->btn_disruptLight, SIGNAL(released()),
            this, SLOT(disruptLightSensor()));
    connect(ui->btn_fireTest, SIGNAL(released()),
            this, SLOT(fireSensor()));
    connect(ui->btn_powerTest, SIGNAL(released()),
            this, SLOT(powerOutageSensor()));

    //Floor buttons and create passenger when pressed//
    //Floor 1
    connect(ui->btn_f1u, SIGNAL(released()),
            ecs->getFloor(0), SLOT(createPassenger()));
    connect(ui->btn_f1u, SIGNAL(released()),
            ecs->getFloor(0)->getButton(FLR_UP), SLOT(sendSignal()));
    //Floor 2
    connect(ui->btn_f2u, SIGNAL(released()),
            ecs->getFloor(1), SLOT(createPassenger()));
    connect(ui->btn_f2d, SIGNAL(released()),
            ecs->getFloor(1), SLOT(createPassenger()));
    connect(ui->btn_f2u, SIGNAL(released()),
            ecs->getFloor(1)->getButton(FLR_UP), SLOT(sendSignal()));
    connect(ui->btn_f2d, SIGNAL(released()),
            ecs->getFloor(1)->getButton(FLR_DOWN), SLOT(sendSignal()));
    //Floor 3
    connect(ui->btn_f3u, SIGNAL(released()),
            ecs->getFloor(2), SLOT(createPassenger()));
    connect(ui->btn_f3d, SIGNAL(released()),
            ecs->getFloor(2), SLOT(createPassenger()));
    connect(ui->btn_f3u, SIGNAL(released()),
            ecs->getFloor(2)->getButton(FLR_UP), SLOT(sendSignal()));
    connect(ui->btn_f3d, SIGNAL(released()),
            ecs->getFloor(2)->getButton(FLR_DOWN), SLOT(sendSignal()));
    //Floor 4
    connect(ui->btn_f4u, SIGNAL(released()),
            ecs->getFloor(3), SLOT(createPassenger()));
    connect(ui->btn_f4d, SIGNAL(released()),
            ecs->getFloor(3), SLOT(createPassenger()));
    connect(ui->btn_f4u, SIGNAL(released()),
            ecs->getFloor(3)->getButton(FLR_UP), SLOT(sendSignal()));
    connect(ui->btn_f4d, SIGNAL(released()),
            ecs->getFloor(3)->getButton(FLR_DOWN), SLOT(sendSignal()));
    //Floor 5
    connect(ui->btn_f5u, SIGNAL(released()),
            ecs->getFloor(4), SLOT(createPassenger()));
    connect(ui->btn_f5d, SIGNAL(released()),
            ecs->getFloor(4), SLOT(createPassenger()));
    connect(ui->btn_f5u, SIGNAL(released()),
            ecs->getFloor(4)->getButton(FLR_UP), SLOT(sendSignal()));
    connect(ui->btn_f5d, SIGNAL(released()),
            ecs->getFloor(4)->getButton(FLR_DOWN), SLOT(sendSignal()));
    //Floor 6
    connect(ui->btn_f6u, SIGNAL(released()),
            ecs->getFloor(5), SLOT(createPassenger()));
    connect(ui->btn_f6d, SIGNAL(released()),
            ecs->getFloor(5), SLOT(createPassenger()));
    connect(ui->btn_f6u, SIGNAL(released()),
            ecs->getFloor(5)->getButton(FLR_UP), SLOT(sendSignal()));
    connect(ui->btn_f6d, SIGNAL(released()),
            ecs->getFloor(5)->getButton(FLR_DOWN), SLOT(sendSignal()));
    //Floor 7
    connect(ui->btn_f7d, SIGNAL(released()),
            ecs->getFloor(6), SLOT(createPassenger()));
    connect(ui->btn_f7d, SIGNAL(released()),
            ecs->getFloor(6)->getButton(FLR_DOWN), SLOT(sendSignal()));

}

void MainWindow::addPassengerToComboBox(tuple<int, int, int> p){

    ui->cb_passengerSelect->addItem(QString("%3").arg(get<0>(p)));

}

void MainWindow::removePassengerFromComboBox(int id){

    QString passenger = QString("%3").arg(id);
    int index = ui->cb_passengerSelect->findText(passenger);
    ui->cb_passengerSelect->removeItem(index);

}

void MainWindow::floor1Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 1);
    }
}

void MainWindow::floor2Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 2);
    }
}

void MainWindow::floor3Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 3);
    }
}

void MainWindow::floor4Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 4);
    }
}

void MainWindow::floor5Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 5);
    }
}

void MainWindow::floor6Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 6);
    }
}

void MainWindow::floor7Request(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->setPassengerFloorDest(elvNum, id, 7);
    }
}

void MainWindow::helpRequest(){
    //Get elevator passenger is on
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->requestInElevator(elvNum, SIG_HELP, id);
    }
}

void MainWindow::closeDoorRequest(){
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->requestInElevator(elvNum, SIG_CLOSE_DOOR, id);
    }
    else{
        qInfo("No passengers in elevator to close door");
    }
}

void MainWindow::openDoorRequest(){
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ecs->requestInElevator(elvNum, SIG_OPEN_DOOR, id);
    }
    else{
        qInfo("No passengers in elevator to open door");
    }
}

void MainWindow::disruptLightSensor(){
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);

    if(elvNum != 0){
        ecs->requestInElevator(elvNum, SIG_OBSTACLE, id);
    }
    else{
        qInfo("No passengers in elevator to disrupt light sensor");
    }

}

void MainWindow::fireSensor(){
    ecs->emergency(SIG_FIRE);
}

void MainWindow::powerOutageSensor(){
    ecs->emergency(SIG_POWER_OUTAGE);
}

void MainWindow::updateDisplay(){
    int id = ui->cb_passengerSelect->currentText().toInt();
    int elvNum = ecs->getCurrentElevatorOfPassenger(id);
    if(elvNum != 0){
        ui->lbl_DisplaySystem->setText(
            QString::fromStdString(ecs->getElevator(elvNum - 1)->getDisplayText()));

    }
    else{
        ui->lbl_DisplaySystem->setText(QString(""));
    }
}

void MainWindow::updateElevator(){
    int elevatorNum = ui->cb_elevatorNum->currentText().toInt();
    Elevator* elevator = ecs->getElevator(elevatorNum - 1);

    int status = elevator->getState();
    QString overloadWeight = QString("%5").arg(elevator->getMaxWeight());
    QString currWeight = QString("%5").arg(elevator->getCurrentWeight());;
    QString currFloor = QString("%3").arg(elevator->getCurrFloor());
    QString display = QString::fromStdString(elevator->getDisplayText());

    QString strStatus;
    if(status == STA_IDLE){
        strStatus = "Idle";
    }
    else if(status == STA_MOVING_UP){
        strStatus = "Moving up";
    }
    else if(status == STA_MOVING_DOWN){
        strStatus = "Moving down";
    }
    else if(status == STA_STOP){
        strStatus = "Stopped";
    }
    else if(status == STA_OPEN){
        strStatus = "Doors opened";
    }
    else if(status == STA_CLOSING){
        strStatus = "Doors closing";
    }
    else if(status == STA_CLOSE){
        strStatus = "Doors closed";
    }
    else if(status == STA_READY){
        strStatus = "Ready to move";
    }
    else if(status == STA_EMERGENCY){
        strStatus = "Emergency";
    }
    else if(status == STA_OVERLOAD){
        strStatus = "Overload";
    }
    else if(status == STA_OBSTACLE){
        strStatus = "Obstacle";
    }
    else{
        strStatus = "OFFLINE";
    }

    ui->lbl_elvStatus->setText(strStatus);
    ui->lbl_elvOverloadWeight->setText(overloadWeight);
    ui->lbl_elvCurrWeight->setText(currWeight);
    ui->lbl_elvCurrFloor->setText(currFloor);
    ui->lbl_elvDisplaySystem->setText(display);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete timeInterval;
    delete ecs;
}
