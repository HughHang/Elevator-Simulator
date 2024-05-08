#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "ElevatorControlSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addPassengerToComboBox(tuple<int, int, int>);
    void removePassengerFromComboBox(int);
    void floor1Request();
    void floor2Request();
    void floor3Request();
    void floor4Request();
    void floor5Request();
    void floor6Request();
    void floor7Request();
    void helpRequest();
    void closeDoorRequest();
    void openDoorRequest();
    void disruptLightSensor();
    void fireSensor();
    void powerOutageSensor();
    void updateDisplay();
    void updateElevator();

signals:
    void passengerChooseFloor(int, int, int);

private:
    Ui::MainWindow *ui;

    ElevatorControlSystem*   ecs;
    QTimer* timeInterval;
    static int interval;


};
#endif // MAINWINDOW_H
