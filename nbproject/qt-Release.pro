# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = QRadioProfile
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets
SOURCES += Importer.cpp Point.cpp ProcessParams.cpp Processor.cpp RightTriangle.cpp Vector.cpp main.cpp
HEADERS += Importer.h Point.h ProcessParams.h Processor.h RightTriangle.h Vector.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
