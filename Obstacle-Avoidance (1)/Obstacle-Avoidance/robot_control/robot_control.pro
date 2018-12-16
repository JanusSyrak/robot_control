TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fuzzybugcontroller.cpp \
    laserscanner.cpp

CONFIG += link_pkgconfig
PKGCONFIG += gazebo
PKGCONFIG += opencv

HEADERS += \
    fuzzybugcontroller.h \
    laserscanner.h

DISTFILES += \
    fuzzybugcontroller.fll




INCLUDEPATH+=/home/agervig/Desktop/fuzzylite-6.0/fuzzylite
LIBS+=-L/home/agervig/Desktop/fuzzylite-6.0/release/bin -lfuzzylite-static
