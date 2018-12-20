#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T20:54:01
#
#-------------------------------------------------

QT       += core gui
CONFIG   += debug

LIBS     += -lopencv_video -lopencv_core -lopencv_highgui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gorilla-presenter
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    goprshader.cc \
    goprgeometrybuffer.cc \
    goprpresentation.cc \
    goprplayer.cc \
    goprabstractrenderer.cc \
    goprstandardrenderer.cc \
    goprabstractsource.cc \
    goprfilesource.cc \
    transitions/goprtransition.cc \
    transitions/goprslidetransition.cc \
    transitions/goprcubetransition.cc \
    gopropenglviewer.cc \
    transitions/goprrotatetransition.cc \
    goprstackmodel.cc \
    goprcontroller.cc \
    goprlogo.cc \
    goprlogoanimation.cc \
    goprtextdocument.cc \
    streams/gopropencvstream.cc \
    streams/goprimagestream.cc \
    streams/gopropencvsource.cc \
    utils/goprterminator.cc

HEADERS  += \
    goprshader.h \
    goprgeometrybuffer.h \
    goprpresentation.h \
    goprplayer.h \
    goprabstractrenderer.h \
    goprstandardrenderer.h \
    goprabstractsource.h \
    goprfilesource.h \
    transitions/goprtransition.h \
    transitions/goprslidetransition.h \
    transitions/goprcubetransition.h \
    gopropenglviewer.h \
    transitions/goprrotatetransition.h \
    goprstackmodel.h \
    goprcontroller.h \
    goprlogo.h \
    goprlogoanimation.h \
    goprtextdocument.h \
    streams/gopropencvstream.h \
    streams/goprimagestream.h \
    streams/gopropencvsource.h \
    utils/goprterminator.h

FORMS    += gopreditor.ui

RESOURCES += \
    resources.qrc

