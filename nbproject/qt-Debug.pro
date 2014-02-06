# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = QRadioProfile
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets
SOURCES += src/geometry/RightTriangle.cpp src/gui/FilePicker.cpp src/gui/MainWindow.cpp src/gui/ParamsWidget.cpp src/import/Importer.cpp src/main.cpp src/processing/ProcessParams.cpp src/processing/Processor.cpp
HEADERS += src/geometry/RightTriangle.h src/gui/FilePicker.h src/gui/MainWindow.h src/gui/ParamsWidget.h src/import/Importer.h src/processing/ProcessParams.h src/processing/Processor.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += src 
LIBS += 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
