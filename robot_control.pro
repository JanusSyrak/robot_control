    TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fuzzybugcontroller.cpp \
    laserscanner.cpp

INCLUDEPATH += /home/stefan/Desktop/Project/fuzzylite-6.0-linux64/fuzzylite-6.0/fuzzylite

LIBS += -L/home/stefan/Desktop/Project/fuzzylite-6.0-linux64/fuzzylite-6.0/release/bin -lfuzzylite-static


CONFIG += link_pkgconfig
PKGCONFIG += gazebo
PKGCONFIG += opencv

DISTFILES += \
    fuzzybugcontroller.fll

HEADERS += \
    fuzzybugcontroller.h \
    laserscanner.h
