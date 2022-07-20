#-------------------------------------------------
#
# Project created by QtCreator 2022-07-20T22:49:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sortThread
TEMPLATE = app
DESTDIR_TARGET = $$PWD/../bin
OBJECTS_DIR = .obj
RCC_DIR = .rcc
MOC_DIR = .moc
UI_DIR = .uic

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mythread.cpp

HEADERS += \
        mainwindow.h \
    mythread.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
