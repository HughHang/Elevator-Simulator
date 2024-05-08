QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AudioSystem.cpp \
    BuildingSafetyServices.cpp \
    Button.cpp \
    DisplaySystem.cpp \
    Elevator.cpp \
    ElevatorButton.cpp \
    ElevatorControlSystem.cpp \
    Floor.cpp \
    FloorButton.cpp \
    Passenger.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AudioSystem.h \
    BuildingSafetyServices.h \
    Button.h \
    DisplaySystem.h \
    Elevator.h \
    ElevatorButton.h \
    ElevatorControlSystem.h \
    Floor.h \
    FloorButton.h \
    Passenger.h \
    defs.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    ElevatorSim_en_CA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
