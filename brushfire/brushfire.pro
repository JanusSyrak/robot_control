TARGET = ex1
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    wavefront.cpp \
    main.cpp \
    brushfire.cpp \
    gradient.cpp \
    mapgenerator.cpp

# Linux
unix:!macx {
    # The path to opencv.pc must be in the environment variable PKG_CONFIG_PATH. You can add it in the kit configuration.
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

# Windows
win32 {

    # opencv
    INCLUDEPATH += C:\opencv\build\include

    CONFIG(debug,debug|release){
        #debug
        LIBS += -LC:\opencv\build\x64\vc15\lib -lopencv_world343d
    } else {
        #release
        LIBS += -LC:\opencv\build\x64\vc15\lib -lopencv_world343
    }
}

# Mac OS X
macx {

    # Using pkg-config to get the correct compiler and linker flags.
    # The path to opencv.pc must be in the environment variable PKG_CONFIG_PATH. You can add it in the kit configuration.
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv

    # Alternative: manually setting path to include files and linker flags
    #INCLUDEPATH += /usr/local/include
    #LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs
}

HEADERS += \
    wavefront.h \
    brushfire.h \
    gradient.h \
    mapgenerator.h
