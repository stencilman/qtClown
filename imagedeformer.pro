QT += opengl
TARGET = imagedeformer
CONFIG += debug_and_release \
    console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += imagedeformer.cpp \
    Anchor.cpp \
    SceneViewer.cpp \
    glextensions.cpp \
    AnchorStorageMgr.cpp \
    MainWindow.cpp \
    glshaders.cpp
HEADERS += Anchor.h \
    MainWindow.h \
    glshaders.h \
    AnchorStorageMgr.h \
    SceneViewer.h \
    glextensions.h
INCLUDEPATH += 
RESOURCES += imagedeformer.qrc
unix { 
    message (qmake uses the unix setting)
    SYSTEM_NAME = Linux6
    CONFIG(debug, debug|release) { 
        OBJECTS_DIR = ./$(SYSTEM_NAME)/Debug
        MOC_DIR = ./$(SYSTEM_NAME)/Debug
        DESTDIR = ./$(SYSTEM_NAME)/Debug
        RCC_DIR = ./
    }
    else { 
        OBJECTS_DIR = ./$(SYSTEM_NAME)/Release
        MOC_DIR = ./$(SYSTEM_NAME)/Release
        DESTDIR = ./$(SYSTEM_NAME)/Release
        RCC_DIR = ./
    }
}
win32 { 
    message (qmake uses the win32-minw setting)
    SYSTEM_NAME = win32-minw
}
