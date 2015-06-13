#-------------------------------------------------
#
# Project created by QtCreator 2014-12-31T16:51:01
#
#-------------------------------------------------

QT       += core gui network webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = VKTalks
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    kernel.cpp \
    settingsmanager.cpp \
    authorizationmanager.cpp \
    usermanager.cpp

HEADERS  += mainwindow.h \
    kernel.h \
    customtypes.h \
    settingsmanager.h \
    authorizationmanager.h \
    user.h \
    usermanager.h

FORMS    += mainwindow.ui
